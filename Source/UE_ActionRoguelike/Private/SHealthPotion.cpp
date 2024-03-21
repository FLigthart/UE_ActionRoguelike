#include "SHealthPotion.h"

#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
{
	RespawnTime = 10.f;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn)) return;

	USAttributeComponent* InstigatorAttributeComp = InstigatorPawn->GetComponentByClass<USAttributeComponent>();
	if (InstigatorAttributeComp && !InstigatorAttributeComp->IsMaxHealth())
	{
		ISGameplayInterface::Interact_Implementation(InstigatorPawn);

		InstigatorAttributeComp->ApplyHealthChange(this, 25.0f);
		
		Disable();
	}
}


