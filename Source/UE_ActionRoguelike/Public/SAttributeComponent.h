#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewRage, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
	USAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float HealthMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float RageMax;

	// The percentage of the damage taken that is converted into rage (e.g. 0.5 = half of the damage taken is given as rage).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float RagePercentage;
	
public:	

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);
	
	UPROPERTY(BlueprintAssignable, Transient)
	FOnHealthChanged OnHealthChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	FORCEINLINE float GetHealth() const { return Health; };
	FORCEINLINE float GetHealthMax() const { return HealthMax; };
	FORCEINLINE bool IsMaxHealth() const { return FMath::IsNearlyEqual(Health, HealthMax); };

	UPROPERTY(BlueprintAssignable, Transient)
	FOnRageChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	// This function must be used to "buy" an action that costs rage. It calculates if the instigator
	// has the required amount of rage, and applies the rage change if it has.
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool RequestRageAction(AActor* InstigatorActor, float Cost);
	
	FORCEINLINE float GetRage() const { return Rage; };
	FORCEINLINE float GetRageMax() const { return RageMax; };
	FORCEINLINE bool IsMaxRage() const { return FMath::IsNearlyEqual(Rage, RageMax);}
};
