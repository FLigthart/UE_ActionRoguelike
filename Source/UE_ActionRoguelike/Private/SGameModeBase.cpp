#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SMonsterData.h"
#include "SPlayerState.h"
#include "SPowerupActor.h"
#include "AI/SAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SSaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UE_ActionRoguelike/UE_ActionRoguelike.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ar.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;

	MaxBotCount = 10;

	RespawnDelay = 2.0f;

	PlayerStateClass = ASPlayerState::StaticClass();

	CreditsForKill = 20;

	SaveSlotName = "SaveGame01";
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	if (SelectedSaveSlot.Len() > 0)	// Is SelectedSaveSlot specified?
	{
		SaveSlotName = SelectedSaveSlot;
	}

	LoadSaveGame();
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
	
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	LoadObjectsFromSave();
	
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
	SpawnPowerups();
}

/*
 *	AI Spawn
 */
void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
	
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) //Count number of bots in level
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}
	
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (NrOfAliveBots >= MaxBotCount) //Maximum amount of bots reached. So don't run query.
	{
		return;
	}
	
	FEnvQueryRequest Request(SpawnBotQuery, this);
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &ASGameModeBase::OnBotSpawnQueryCompleted);
}

void ASGameModeBase::OnBotSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query Failed!"));
		return;
	}
	
	// Retrieve all possible locations that passed the query
	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);

	if (Locations.IsValidIndex(0) && MonsterTable)
	{
		TArray<FMonsterInfoRow*> Rows;
		MonsterTable->GetAllRows("",  Rows);

		int32 RandomIndex = FMath::RandRange(0, Rows.Num() -  1);
		FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

		// Async load units
		UAssetManager& Manager = UAssetManager::Get();
		
		TArray<FName> Bundles;	// Bundles can allow you to only load parts of a primary asset (e.g. UI bundle with only an icon, in-game bundle with all gameplay related assets, etc.)

		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnMonsterLoaded, SelectedRow->MonsterID, Locations[0]);
		
		Manager.LoadPrimaryAsset(SelectedRow->MonsterID, Bundles, Delegate);	//OnMonsterLoaded continues spawning when monster is loaded.
	}
}

void ASGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedID, FVector SpawnLocation)
{
	UAssetManager& Manager = UAssetManager::Get();
	
	USMonsterData* MonsterData = CastChecked<USMonsterData>(Manager.GetPrimaryAssetObject(LoadedID));

	if (MonsterData)
	{
		AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
		if (NewBot)
		{
			LogOnScreen(this, FString::Printf(TEXT("Spawnded Enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

			USActionComponent* ActionComp = Cast<USActionComponent>(NewBot->GetComponentByClass(USActionComponent::StaticClass()));
			if (ActionComp)
			{
				for (TSubclassOf<USAction> ActionClass : MonsterData->Actions)	// Add all Actions in the MonsterData to the unit.
				{
					ActionComp->AddAction(NewBot, ActionClass);
				}
			}
		}
	}
}

/*
 *	Spawn Powerups
 */

void ASGameModeBase::SpawnPowerups()
{
	FEnvQueryRequest Request(PowerupsQuery, this);
	Request.Execute(EEnvQueryRunMode::AllMatching, this, &ASGameModeBase::OnPowerupSpawnQueryCompleted);
}

void ASGameModeBase::OnPowerupSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Powerup Spawn EQS Query Failed!"));
		return;
	}
	
	TArray<FVector> UnusedLocations; //Locations that do not already have an Powerup spawned on it.
	QueryResult->GetAllAsLocations(UnusedLocations);
	
	for (TSubclassOf<ASPowerupActor> PowerupClass : PowerupClasses)
	{
		ASPowerupActor* PowerupActor = PowerupClass->GetDefaultObject<ASPowerupActor>();
		if (ensure(PowerupActor))
		{
			TArray<int32> UsedLocations = SpawnPowerupsOfType(PowerupClass, PowerupActor->GetPowerupSpawnRate(), UnusedLocations);
			for (int32 UsedLocation : UsedLocations)
			{
				UnusedLocations.RemoveAt(UsedLocation);
			}
		}
	}
}

TArray<int32> ASGameModeBase::SpawnPowerupsOfType(TSubclassOf<ASPowerupActor> PowerupType, float SpawnRate, TArray<FVector> Locations)
{
	TArray<int32> UsedLocations;

	int32 PowerupCount = FMath::RoundToInt32(Locations.Num() * SpawnRate);
	
	while (UsedLocations.Num() < PowerupCount) //Spawn Powerups
	{
		if (Locations.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Insufficient amount of locations available to spawn all powerups."));
			break;
		}
		
		int32 LocationIndex = FMath::RandRange(0, Locations.Num() - 1);
		FVector SpawnLocation = Locations[LocationIndex];
		
		GetWorld()->SpawnActor<ASPowerupActor>(PowerupType, SpawnLocation, FRotator::ZeroRotator);

		Locations.RemoveAt(LocationIndex); //Location can't be used more than once
		
		UsedLocations.Add(LocationIndex);
	}

	return UsedLocations;
}

/*
 *	GameObject Spawns
 */
void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	if (ASCharacter* Player = Cast<ASCharacter>(VictimActor))
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
		
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn && KillerPawn != VictimActor) //Victim can't be the killer
	{
		ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>();
		if (PS && PS != VictimActor) //AI will be nullptr.
		{
			PS->AddCredits(CreditsForKill);
		}
	}
}

/*
 *	Console Commands
 */	
void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) //Count number of bots in level
		{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // TODO: Pass in player(?) for kill credit
		}
	}
}

/*
 *	Save Game Functions
 */

void ASGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;	//Singe Player only
		}
	}

	CurrentSaveGame->SavedActors.Empty();
	
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		// Skip non-gameplay actors
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		/*
		 *	Save variables in classes with UPROPERTY SaveGame
		 */
		
		// Pass the array to fill with data as bytes
		FMemoryWriter MemWriter(ActorData.ByteData);
		
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Only find variables with UPROPERTY SaveGame.
		Ar.ArIsSaveGame = true;
		
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
}

/*
 *	It's crucial that both LoadSaveGame() and LoadObjectsFromSave() are called. LoadSaveGame() needs to happen in Init() and LoadObjectsFromSave in StartPlay().
 */
void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data File."));
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame Data."));
	}
}

void ASGameModeBase::LoadObjectsFromSave()
{
	if (CurrentSaveGame == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
		return;
	}

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
			
		// Skip non-gameplay actors
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}
			
		for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
		{	
			if (ActorData.ActorName == Actor->GetName())
			{
				Actor->SetActorTransform(ActorData.Transform);


				/*
				 *	Load variables in classes with UPROPERTY SaveGame
				 */
					
				FMemoryReader MemReader(ActorData.ByteData);

				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
				// Only find variables with UPROPERTY SaveGame.
				Ar.ArIsSaveGame = true;
		
				Actor->Serialize(Ar);

				// Trigger load function on Actors with SGameplayInterface.
				ISGameplayInterface::Execute_OnActorLoaded(Actor);
					
				break;
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data Objects into world."));
}
