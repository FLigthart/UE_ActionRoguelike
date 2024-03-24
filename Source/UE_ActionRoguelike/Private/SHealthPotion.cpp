#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

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


