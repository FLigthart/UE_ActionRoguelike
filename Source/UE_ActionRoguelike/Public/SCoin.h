#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SCoin.generated.h"

class USItemRotatingMovementComp;

UCLASS()
class UE_ACTIONROGUELIKE_API ASCoin : public ASPowerupActor
{
	GENERATED_BODY()

public:
	ASCoin();
	
protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USItemRotatingMovementComp> RotatingComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditsAmount;
};
