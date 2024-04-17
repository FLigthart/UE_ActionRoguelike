#pragma once

#include "CoreMinimal.h"
#include "SAction_ProjectileAttack.h"
#include "SAction_PrimaryAttack.generated.h"

UCLASS()
class UE_ACTIONROGUELIKE_API USAction_PrimaryAttack : public USAction_ProjectileAttack
{
	GENERATED_BODY()

public:
	
	USAction_PrimaryAttack();

	virtual void StartAction_Implementation(AActor* Instigator) override;
};
