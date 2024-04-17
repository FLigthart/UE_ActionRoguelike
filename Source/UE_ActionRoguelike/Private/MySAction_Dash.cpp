#include "MySAction_Dash.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


UMySAction_Dash::UMySAction_Dash()
{
	AttackDelay = 0.44f;
	DashAbilityExitDelay = 0.2f;
}

void UMySAction_Dash::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	UGameplayStatics::SpawnEmitterAttached(CastEffect, Instigator->GetMesh(), SpawnSocket, FVector::ZeroVector,
FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		
	Instigator->PlayAnimMontage(TeleportMontage, 1, "IntroStart");
}

void UMySAction_Dash::AttackDelay_Elapsed()
{
	Super::AttackDelay_Elapsed();

	FTimerHandle TimerHandle_Exit;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Exit, this, &UMySAction_Dash::DashAbility_Exit, DashAbilityExitDelay);
}

void UMySAction_Dash::DashAbility_Exit()
{
	Instigator->PlayAnimMontage(TeleportMontage, 1, "EndStart");
}
