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
	if (ensureAlways(ProjectileClass))
	{
		//This is the start location of the actual projectile.
		FVector HandLocation = Instigator->GetMesh()->GetSocketLocation(SpawnSocket);

		FTransform SpawnTransform;
		FActorSpawnParameters SpawnParams;
		CalculateSpawnParams(HandLocation, &SpawnTransform, &SpawnParams, 5000.f, false);
		
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
	}

	FTimerHandle TimerHandle_Exit;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Exit, this, &UMySAction_Dash::DashAbility_Exit, DashAbilityExitDelay);
}

void UMySAction_Dash::DashAbility_Exit()
{
	Instigator->PlayAnimMontage(TeleportMontage, 1, "EndStart");

	StopAction(Instigator);
}
