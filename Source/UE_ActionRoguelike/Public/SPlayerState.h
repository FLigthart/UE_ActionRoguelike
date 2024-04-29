#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

UCLASS()
class UE_ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASPlayerState();
	
protected:

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_Credits", Category = "Credits")
	int32 CreditAmount;

	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

public:
	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCreditAmount() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;
};
