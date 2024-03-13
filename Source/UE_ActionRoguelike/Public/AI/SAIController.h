
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAIController.generated.h"

UCLASS()
class UE_ACTIONROGUELIKE_API ASAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviourTree;



	
	virtual void BeginPlay() override;

};
