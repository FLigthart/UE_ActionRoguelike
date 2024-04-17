#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SCharInfoInterface.generated.h"

UINTERFACE(MinimalAPI)
class USCharInfoInterface : public UInterface
{
	GENERATED_BODY()
};

// Declare interface functions here
class UE_ACTIONROGUELIKE_API ISCharInfoInterface
{
	GENERATED_BODY()

public:
	/* Get the CameraComp Transform from SCharacter */
	virtual FTransform GetCameraTransform() const = 0;
};
