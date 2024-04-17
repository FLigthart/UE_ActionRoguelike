#pragma once

#include "CoreMinimal.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "SItemRotatingMovementComp.generated.h"

UCLASS()
class UE_ACTIONROGUELIKE_API USItemRotatingMovementComp : public URotatingMovementComponent
{
	GENERATED_BODY()

public:
	USItemRotatingMovementComp();

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// The speed that the item rotates.
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float YawRate;

	// The speed that the item goes up and down.
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MovementSpeed;

	//The amount that the item goes up and down.
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MovementLength;

	//Whether the owner should move up and down.
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	bool bShouldMove;
};
