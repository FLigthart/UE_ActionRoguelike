#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGameplayInterface.generated.h"

//This class does not need to be modified. Boilerplate for UE.
UINTERFACE(MinimalAPI)
class USGameplayInterface : public UInterface
{
	GENERATED_BODY()
	
};

//Add interface functions here.
class UE_ACTIONROGUELIKE_API ISGameplayInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
	
	FORCEINLINE void SetCanInteract(bool Value) { bCanInteract = Value; };
	FORCEINLINE bool GetCanInteract() { return bCanInteract; };

private:
	bool bCanInteract = true;
};
