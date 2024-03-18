#include "SDashProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
	DetonateDelay = 0.2f;
	TeleportDelay = 0.2f;
	
	MovementComp->InitialSpeed = 6000.f;

	CharacterInstigator = nullptr;
	
	bShouldInterpolate = false;

	DashDuration = 0.1f;
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
	CharacterInstigator = Cast<APawn>(GetInstigator());
	
	if (ensure(CharacterInstigator))
	{
		OldLocation = CharacterInstigator->GetActorLocation(); //Location of pawn
		NewLocation = GetActorLocation(); //Location of dash projectile

		StartTime = GetWorld()->GetTimeSeconds();

		EffectComp->Deactivate();
		
		bShouldInterpolate = true;
	}
}

void ASDashProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bShouldInterpolate) //Should character "slowly" teleport
	{
		float Alpha = (GetWorld()->GetTimeSeconds() - StartTime) / DashDuration;
		
		CharacterInstigator->TeleportTo(FMath::Lerp(OldLocation, NewLocation, Alpha), CharacterInstigator->GetActorRotation(), false, false);
		
		if (FMath::IsNearlyEqual(Alpha, 1) || Alpha > 1.0f) //1 == 100%, so interpolating finished.
		{
			DashAbility_Finished();
			bShouldInterpolate = false;
		}
	}
}

void ASDashProjectile::DashAbility_Finished()
{
	APlayerController* PC = Cast<APlayerController>(CharacterInstigator->GetController());
	if (PC && PC->IsLocalController())
	{
		PC->ClientStartCameraShake(CameraShakeAttack);
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportExit, FTransform(CharacterInstigator->GetActorRotation(),
	CharacterInstigator->GetActorLocation() + CharacterInstigator->GetActorForwardVector() * 50.f));
	
	Destroy();
}


