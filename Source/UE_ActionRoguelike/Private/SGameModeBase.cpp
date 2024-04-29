#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "SPowerupActor.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ar.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;

	MaxBotCount = 10;

	RespawnDelay = 2.0f;

	PlayerStateClass = ASPlayerState::StaticClass();

	CreditsForKill = 20;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

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

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
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