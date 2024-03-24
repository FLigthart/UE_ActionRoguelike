#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SCoin.generated.h"

UCLASS()
class UE_ACTIONROGUELIKE_API ASCoin : public ASPowerupActor
{
	GENERATED_BODY()

public:
	ASCoin();

	virtual void BeginPlay() override;
protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditsAmount;
};
