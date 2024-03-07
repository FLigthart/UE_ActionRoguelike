#include "SDashProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASDashProjectile::ASDashProjectile()
{
	DetonateDelay = 0.2f;
	TeleportDelay = 0.2f;
	
	MovementComp->InitialSpeed = 6000.f;
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	GetWorldTimerManager().SetTimer(TimerHandle_DashExplode, this, &ASDashProjectile::Explode, 0.2f);
}

void ASDashProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DashExplode); //Clear timer if projectile hits something

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	
	GetWorldTimerManager().SetTimer(TimerHandle_DashTeleport, this, &ASDashProjectile::DashAbility_Dash, 0.2f);
}

void ASDashProjectile::DashAbility_Dash()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		//Keep rotation of ActorToTeleport
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);

		Destroy();
	}
}

