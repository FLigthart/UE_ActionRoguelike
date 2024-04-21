#include "SCoin.h"

#include "SItemRotatingMovementComp.h"
#include "SPlayerState.h"

ASCoin::ASCoin()
{
	RotatingComp = CreateDefaultSubobject<USItemRotatingMovementComp>("RotatingComp");
	RotatingComp->YawRate = 50.f;
	
	CreditsAmount = 100;

	PowerupSpawnRate = 0.025f;

	SpawnHeightOffset = 40.f;
}

void ASCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		PS->AddCredits(CreditsAmount);
		Disable();
	}
}
