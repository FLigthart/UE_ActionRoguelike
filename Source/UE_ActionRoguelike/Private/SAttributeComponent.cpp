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
	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, HealthMax);

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}

