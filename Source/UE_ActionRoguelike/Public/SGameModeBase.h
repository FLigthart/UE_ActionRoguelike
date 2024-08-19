#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class USMonsterData;
class ASPowerupActor;
class UEnvQuery;
class USSaveGame;

struct FEnvQueryResult;

//	DataTable Row for spawning monsters in Game Mode.
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost =  5.0f;
		KIllReward = 20.0f;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterID;

	// Relative chance to pick this unit.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	// Points required by Gamemode to spawn this unit.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	// Amount of credits awarded to killer of this unit.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KIllReward;
};

UCLASS()
class UE_ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	ASGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	
	/*
	 *	Save and Load Game
	 */

	FString SaveSlotName;
	
	UPROPERTY()
	USSaveGame* CurrentSaveGame;
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	// Load the save file, this needs to happen before loading player state (credits)
	void LoadSaveGame();

	// Set Actor states, this needs to happen on StartGame() or later.
	void LoadObjectsFromSave();
	
protected:

	/*
	 *	Minions and Minion Spawning
	 */
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
    TObjectPtr<UEnvQuery> SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UCurveFloat> DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 MaxBotCount;
	
	FTimerHandle TimerHandle_SpawnBots;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UDataTable> MonsterTable;

	UFUNCTION()
	void SpawnBotTimerElapsed();
	
	void OnBotSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);

	void OnMonsterLoaded(FPrimaryAssetId LoadedID, FVector SpawnLocation);

	/*
	 *	Powerups
	 */

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TObjectPtr<UEnvQuery> PowerupsQuery;

	/* All power-up classes used to spawn with EQS at match start */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TArray<TSubclassOf<ASPowerupActor>> PowerupClasses;
	
	UFUNCTION()
	void SpawnPowerups();
	void OnPowerupSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);
	TArray<int32> SpawnPowerupsOfType(TSubclassOf<ASPowerupActor> PowerupType, float SpawnRate, TArray<FVector> Locations); //Spawn Powerups of type and Spawn Rate. Returns used locations.

	/*
	 *	Player Spawning and Killing
	 */
	
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float RespawnDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditsForKill;
};
