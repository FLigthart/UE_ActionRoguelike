#include "SActionEffect_Thorns.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	ThornPercentage = 0.05f;
	
	Duration = 0.0f;
	Period = 0.0f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// Only damage, and should not apply on damage to self.
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		int32 RoundedDamage = FMath::RoundToInt(Delta * ThornPercentage);
		
		if (RoundedDamage == 0)
		{
			return;
		}

		RoundedDamage = FMath::Abs(RoundedDamage);

		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, RoundedDamage);
	}
}
