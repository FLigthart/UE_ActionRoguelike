#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"


class USAttributeComponent;

UCLASS()
class UE_ACTIONROGUELIKE_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()

public:
	
	USActionEffect_Thorns();

protected:

	// The percentage of damage redirected at the instigator.
	UPROPERTY(EditDefaultsOnly, Category = "Effects", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float ThornPercentage;

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};
