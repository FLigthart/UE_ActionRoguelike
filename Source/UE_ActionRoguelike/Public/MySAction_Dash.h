#pragma once

#include "CoreMinimal.h"
#include "SAction_ProjectileAttack.h"
#include "MySAction_Dash.generated.h"

UCLASS()
class UE_ACTIONROGUELIKE_API UMySAction_Dash : public USAction_ProjectileAttack
{
	GENERATED_BODY()
	
public:
	UMySAction_Dash();

	bool CanStart_Implementation(AActor* Instigator) override;
	
	void StartAction_Implementation(AActor* Instigator) override;
protected:

	// This tag is the 'voucher' that can be redeemed to use the dash ability.
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTag DashTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TObjectPtr<UAnimMontage> TeleportMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float DashAbilityExitDelay;

	virtual void AttackDelay_Elapsed() override;
	
	void DashAbility_Exit();
};
