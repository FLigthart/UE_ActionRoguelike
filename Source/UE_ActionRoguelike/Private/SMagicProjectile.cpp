#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASMagicProjectile::ASMagicProjectile()
{
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(SphereComp);
	
	PrimaryActorTick.bCanEverTick = true;
	
	MovementComp->InitialSpeed = 8000.f;
	MovementComp->MaxSpeed = 8000.f;

	DamageAmount = 20.f;
}

void ASMagicProjectile::SetDamageAmount(float Damage)
{
	DamageAmount = Damage;
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap); //If actor gets it
}


void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator()) //If actor gets hit. Should ignore Instigator and other projectiles.
	{
		//TODO: Ignore Projectile (Collision Channel?)
		//Apply damage with a bit of force
		USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult);
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());
	
		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
		}
		
		Destroy();
	}
}

void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->MoveIgnoreActors.AddUnique(GetInstigator());
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASMagicProjectile::DestroyProjectile, 4.0f, false);
}

void ASMagicProjectile::DestroyProjectile()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
	SetLifeSpan(0.2f);
}