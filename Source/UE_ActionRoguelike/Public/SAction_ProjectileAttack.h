#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

struct FStreamableHandle;

UCLASS()
class UE_ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, Category = "Attack|Projectile")
	TSoftClassPtr<AActor> ProjectileClass;
	
	TSubclassOf<AActor> Projectile; // The actual loaded projectile. Is loaded at runtime.
	
	TSharedPtr<FStreamableHandle> StreamableHandle;	//StreamableHandle of the projectile
	
	UPROPERTY(VisibleAnywhere, Category = "Sockets")
	FName SpawnSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackDelay;

	UPROPERTY(EditAnywhere, Category = "Attack|Projectile")
	TObjectPtr<UAnimMontage> AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack|Projectile")
	TObjectPtr<UParticleSystem> CastEffect;

	TObjectPtr<ACharacter> Instigator;

	UFUNCTION()
	virtual void AttackDelay_Elapsed();

	void CalculateSpawnParams(FVector SpawnLocation, FTransform* SpawnTransform, FActorSpawnParameters* SpawnTM, float LineTraceLength, bool bIsStraight);
	FVector PerformLineTrace(FTransform Start, float LineTraceLength, bool bIsStraight); //bIsStraight determines if the LineTrace should keep the Z location of the Start vector.

	void LoadProjectileClass(bool bShouldLoadAsynchronous);
	
public:
	
	USAction_ProjectileAttack();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;

};
