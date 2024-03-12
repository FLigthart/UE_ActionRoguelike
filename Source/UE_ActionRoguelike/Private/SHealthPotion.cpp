#include "SHealthPotion.h"

#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!InstigatorPawn) return;

	USAttributeComponent* InstigatorAttributeComp = InstigatorPawn->GetComponentByClass<USAttributeComponent>();
	if (InstigatorAttributeComp && !InstigatorAttributeComp->IsMaxHealth())
	{
		ISGameplayInterface::Interact_Implementation(InstigatorPawn);

		InstigatorAttributeComp->ApplyHealthChange(25.0f);
		
		Disable();
	}
}

void ASHealthPotion::Disable()
{
	MeshComp->SetVisibility(false, true);

	SetCanInteract(false);
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASHealthPotion::Reactivate, 10.f, false);
}

void ASHealthPotion::Reactivate()
{
	MeshComp->SetVisibility(true, true);

	SetCanInteract(true);
}

