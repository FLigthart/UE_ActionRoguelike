#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class UE_ACTIONROGUELIKE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASMagicProjectile();

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> MovementComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> EffectComp;

	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
