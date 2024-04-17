#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS()
class UE_ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* InstigatorActor, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* InstigatorActor, FName ActionName);
	
protected:

	/* Default abilities at game start */
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;
	
	UPROPERTY()
	TArray<TObjectPtr<USAction>> Actions;

	virtual void BeginPlay() override;
};
