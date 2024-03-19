#pragma once

#include "CoreMinimal.h"
#include "BaseAttack.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class UE_ACTIONROGUELIKE_API ASMagicProjectile : public ABaseAttack
{
	GENERATED_BODY()
	
public:	
	ASMagicProjectile();

	void SetDamageAmount(float Damage);
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	float DamageAmount;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAudioComponent> AudioComp;
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
	void DestroyProjectile();
};
