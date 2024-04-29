#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS()
class UE_ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(USAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* InstigatorActor, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* InstigatorActor, FName ActionName);

	USActionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:

	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* InstigatorActor, FName ActionName);
	
	/* Default abilities at game start */
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;
	
	UPROPERTY()
	TArray<TObjectPtr<USAction>> Actions;

	virtual void BeginPlay() override;
};
