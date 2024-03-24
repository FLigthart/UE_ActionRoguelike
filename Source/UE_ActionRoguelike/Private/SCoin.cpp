#include "SCoin.h"

#include "SPlayerState.h"

ASCoin::ASCoin()
{
	CreditsAmount = 100;
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
