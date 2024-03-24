#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.generated.h"

UCLASS()
class UE_ACTIONROGUELIKE_API ASHealthPotion : public ASPowerupActor
{
	GENERATED_BODY()
	
public:	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
	ASHealthPotion();

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditCost;
};
