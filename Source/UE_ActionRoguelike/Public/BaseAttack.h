#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAttack.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class UE_ACTIONROGUELIKE_API ABaseAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseAttack();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UParticleSystemComponent> EffectComp;
	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
