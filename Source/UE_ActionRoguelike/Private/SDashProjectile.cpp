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
	
	AActor* ActorToTeleport = GetInstigator();
	if (ActorToTeleport)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEnter, FTransform(ActorToTeleport->GetActorRotation(),
			ActorToTeleport->GetActorLocation() + ActorToTeleport->GetActorForwardVector() * 50.f));
	}
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	
	GetWorldTimerManager().SetTimer(TimerHandle_DashTeleport, this, &ASDashProjectile::DashAbility_Dash, 0.2f);
}

void ASDashProjectile::DashAbility_Dash()
{
	APawn* CharacterInstigator = Cast<APawn>(GetInstigator());
	if (ensure(CharacterInstigator))
	{
		//Keep rotation of ActorToTeleport
		CharacterInstigator->TeleportTo(GetActorLocation(), CharacterInstigator->GetActorRotation(), false, false);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportExit, FTransform(CharacterInstigator->GetActorRotation(),
			CharacterInstigator->GetActorLocation() + CharacterInstigator->GetActorForwardVector() * 50.f));
		
		APlayerController* PC = Cast<APlayerController>(CharacterInstigator->GetController());
		if (PC && PC->IsLocalController())
		{
			PC->ClientStartCameraShake(CameraShakeAttack);
		}
	}
	Destroy();
}

