#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USWorldUserWidget;
class USActionComponent;
class USAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class UE_ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASAICharacter();

protected:

	TObjectPtr<USWorldUserWidget> ActiveHealthBar;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerSpottedWidget;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAttributeComponent> AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USActionComponent> ActionComp;

	UPROPERTY(VisibleAnywhere, Category = "Parameter Names")
	FName HitFleshColorParamName;
	
	UPROPERTY(VisibleAnywhere, Category = "Parameter Names")
	FName TimeToHitParamName;

	// Key for AI Blackboard is 'TargetActor'
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TargetActorKey;

	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor() const;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};
