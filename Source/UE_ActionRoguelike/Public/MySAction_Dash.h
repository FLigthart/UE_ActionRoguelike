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

	void StartAction_Implementation(AActor* Instigator) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TObjectPtr<UAnimMontage> TeleportMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float DashAbilityExitDelay;

	virtual void AttackDelay_Elapsed() override;
	
	void DashAbility_Exit();
};
