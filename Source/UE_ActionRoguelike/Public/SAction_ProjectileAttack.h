#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

UCLASS()
class UE_ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, Category = "Attack|Projectile")
	TSubclassOf<AActor> ProjectileClass;

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
	
public:
	
	USAction_ProjectileAttack();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;

};
