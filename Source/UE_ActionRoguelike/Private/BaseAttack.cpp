#include "BaseAttack.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABaseAttack::ABaseAttack()
{

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	
	MovementComp->ProjectileGravityScale = 0.f;
}

void ABaseAttack::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &ABaseAttack::OnComponentHit);
}

void ABaseAttack::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ABaseAttack::Explode_Implementation() //Make sure ImpactVFX and ImpactSound are assigned in the child if this function is not overridden.
{
	if (ensure(IsValid(this)))
	{
		if (ImpactVFX)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());
		}

		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
		}

		if (CameraShakeAttack)
		{
			UGameplayStatics::PlayWorldCameraShake(this, CameraShakeAttack, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
		}
		
		Destroy();
	}
}

