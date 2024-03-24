#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

struct FEnvQueryResult;
class UEnvQuery;

UCLASS()
class UE_ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	ASGameModeBase();
	
	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
    TObjectPtr<UEnvQuery> SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UCurveFloat> DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 MaxBotCount;
	
	FTimerHandle TimerHandle_SpawnBots;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UFUNCTION()
	void SpawnBotTimerElapsed();
	
	void OnBotSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float RespawnDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditsForKill;
};
