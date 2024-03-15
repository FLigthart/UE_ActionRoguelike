#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class UE_ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;

	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
};
