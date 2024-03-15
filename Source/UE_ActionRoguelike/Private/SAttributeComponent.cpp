#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{

	Health = 100.f;

	HealthMax = 100.f;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;
	
	Health += Delta;
	
	Health = FMath::Clamp(Health, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);
	
	return ActualDelta != 0;
}

