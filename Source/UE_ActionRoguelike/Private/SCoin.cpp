#include "SCoin.h"

#include "SPlayerState.h"

ASCoin::ASCoin()
{
	CreditsAmount = 100;

	PowerupSpawnRate = 0.025f;
}

void ASCoin::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 40.f));
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
