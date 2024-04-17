#pragma once

#include "CoreMinimal.h"
#include "SAction_ProjectileAttack.h"
#include "SAction_BlackHoleAttack.generated.h"

UCLASS()
class UE_ACTIONROGUELIKE_API USAction_BlackHoleAttack : public USAction_ProjectileAttack
{
	GENERATED_BODY()

public:
	USAction_BlackHoleAttack();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
};
