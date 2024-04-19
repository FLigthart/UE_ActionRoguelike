#include "SAttributeComponent.h"

#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("ar.DamageMultiplier"), 1.0f,
	TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);


USAttributeComponent::USAttributeComponent()
{
	Health = 100.f;
	HealthMax = 100.f;

	Rage = 0.f;
	RageMax = 100.f;
	RagePercentage = 0.5f;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	
	float OldHealth = Health;
	
	Health += Delta;
	
	Health = FMath::Clamp(Health, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	// Apply Rage change if damage taken.
	if (ActualDelta < 0.0f)
	{
		ApplyRageChange(InstigatorActor, FMath::RoundHalfFromZero(FMath::Abs(ActualDelta * RagePercentage)));
	}

	//Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
	return ActualDelta != 0;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	if (Delta > 0.f && IsMaxRage())
	{
		return false;
	}

	float OldRage = Rage;
	
	Rage += Delta;
	
	Rage = FMath::Clamp(Rage, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;

	OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);

	return FMath::IsNearlyZero(ActualDelta);
}

bool USAttributeComponent::RequestRageAction(AActor* InstigatorActor, float Cost)
{
	if (Rage < Cost)
	{
		return false;
	}

	return true;
}
