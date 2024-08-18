#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"


ASHealthPotion::ASHealthPotion()
{
	RespawnTime = 10.f;

	CreditCost = 50;

	PowerupSpawnRate = 0.05f;
}

void ASHealthPotion::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 25.f));
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn)) return;

	USAttributeComponent* InstigatorAttributeComp = InstigatorPawn->GetComponentByClass<USAttributeComponent>();
	if (InstigatorAttributeComp && !InstigatorAttributeComp->IsMaxHealth())
	{
		if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			if (PS->RemoveCredits(CreditCost) && InstigatorAttributeComp->ApplyHealthChange(this, 25.0f))
			{
				ISGameplayInterface::Interact_Implementation(InstigatorPawn);
			
				Disable();
			}
		}
	}
}

FText ASHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComponent && AttributeComponent->IsMaxHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");	//LOCTEXT is the localisation macro.
	}

	ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>();
	if (PS && PS->GetCreditAmount() < CreditCost)
	{
		return FText::Format(LOCTEXT("HealthPotion_InsufficientFundsWarning", "Cost {0} Credits. Insufficient funds for Health Potion."), CreditCost);
	}

	return FText::Format(LOCTEXT("HealthPotion_DefaultInteractMessage", "Cost {0} Credits.  Restores health to maximum."), CreditCost);
}


#undef LOCTEXT_NAMESPACE