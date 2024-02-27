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

protected:

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
