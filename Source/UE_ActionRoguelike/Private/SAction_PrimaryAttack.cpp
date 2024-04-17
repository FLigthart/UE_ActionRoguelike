#include "SAction_PrimaryAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USAction_PrimaryAttack::USAction_PrimaryAttack()
{
	AttackDelay = 0.2f;
}

void USAction_PrimaryAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	
	Instigator->PlayAnimMontage(AttackAnim);

	UGameplayStatics::SpawnEmitterAttached(CastEffect, Instigator->GetMesh(), SpawnSocket, FVector::ZeroVector,
		FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
}
