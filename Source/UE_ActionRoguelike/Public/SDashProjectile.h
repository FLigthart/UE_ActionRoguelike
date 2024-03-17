#pragma once

#include "CoreMinimal.h"
#include "BaseAttack.h"
#include "GameFramework/Actor.h"
#include "SDashProjectile.generated.h"

class UCurveVector;
class UTimelineComponent;

UCLASS()
class UE_ACTIONROGUELIKE_API ASDashProjectile : public ABaseAttack
{
	GENERATED_BODY()
	
public:	
	ASDashProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	TObjectPtr<UParticleSystem> TeleportEnter;
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	TObjectPtr<UParticleSystem> TeleportExit;
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DashDuration;
	
	FTimerHandle TimerHandle_DashExplode;
	FTimerHandle TimerHandle_DashTeleport;

	FVector OldLocation;
	FVector NewLocation;

	bool bShouldInterpolate;

	float StartTime;

	UPROPERTY()
	TObjectPtr<APawn> CharacterInstigator;
	
	virtual void PostInitializeComponents() override;

	virtual void Explode_Implementation() override; //Gets called after 0.2s after the ability is used. Ability Explodes, or when collision
	
	void DashAbility_Dash(); //After another 0.2s the instigator teleports to the location of the projectile.

	virtual void Tick(float DeltaSeconds) override;
	
	void DashAbility_Finished();
};
