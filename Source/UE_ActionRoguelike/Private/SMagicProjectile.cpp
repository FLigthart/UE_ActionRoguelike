#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASMagicProjectile::ASMagicProjectile()
{
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(SphereComp);
	
	PrimaryActorTick.bCanEverTick = true;
	
	MovementComp->InitialSpeed = 1000.f;

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
	if (OtherActor && OtherActor != GetInstigator()) //If actor gets hit. Should ignore projectiles.
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp) //Apply damage if target has Attribute (health) component
		{
			AttributeComp->ApplyHealthChange(-DamageAmount);
			UE_LOG(LogTemp, Warning, TEXT("Minion health: %f"), AttributeComp->GetHealth());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());
		
			if (ImpactSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
			}
			
			Destroy();
		}
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

void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
