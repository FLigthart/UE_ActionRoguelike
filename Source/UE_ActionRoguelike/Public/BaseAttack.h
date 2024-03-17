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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UParticleSystemComponent> EffectComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> ImpactVFX;
	
	UPROPERTY(EditAnywhere, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase> CameraShakeAttack;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeInnerRadius = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeOuterRadius= 500.f;
	
	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void PostInitializeComponents() override;
	
public:	
	virtual void BeginPlay() override;
};
