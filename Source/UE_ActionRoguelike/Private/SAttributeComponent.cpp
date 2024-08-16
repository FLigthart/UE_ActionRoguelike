#include "SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("ar.DamageMultiplier"), 1.0f,
                                                        TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);


USAttributeComponent::USAttributeComponent()
{
	Health = 100.f;
	HealthMax = 100.f;

	Rage = 0.f;
	RageMax = 100.f;
	RagePercentage = 0.5f;

	SetIsReplicatedByDefault(true);
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
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
	
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;

	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

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
	
	float NewRage = Rage + Delta;
	
	NewRage = FMath::Clamp(NewRage, 0.0f, RageMax);

	float ActualDelta = NewRage - OldRage;

	if (GetOwner()->HasAuthority())	//Rage can only be changed on server
	{
		Rage = NewRage;
		
		if (Delta != 0.0f)
		{
			MulticastRageChanged(InstigatorActor, Rage, Delta);
		}
	}
	
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

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);
	
	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, RageMax);
}
