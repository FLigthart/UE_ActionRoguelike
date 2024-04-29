#include "SExplosiveBarrel.h"

#include "SMagicProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASExplosiveBarrel::ASExplosiveBarrel()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RootComponent = StaticMesh;
	
	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForce->SetupAttachment(RootComponent);

	RadialForce->ImpulseStrength = 1000.f;
	RadialForce->Radius = 500.f;
	RadialForce->bImpulseVelChange = true;
	RadialForce->SetAutoActivate(false);

	DamageAmount = -50.f;

	bReplicates = true;
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnHit);
}

void ASExplosiveBarrel::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	if (OtherActor->IsA(ABaseAttack::StaticClass()))
	{
		Explode(OtherActor);
	}
}

void ASExplosiveBarrel::Explode(AActor* DamageCauser)
{
	MulticastExplode();
	
	/*TArray<AActor*> IgnoredActors;
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), DamageAmount, 5.f, GetActorLocation(),
		RadialForce->Radius * 0.25f, RadialForce->Radius, 1.2f, UDamageType::StaticClass(),
		IgnoredActors, DamageCauser);*/

	SetLifeSpan(2.0f);
}

void ASExplosiveBarrel::MulticastExplode_Implementation()
{
	RadialForce->FireImpulse();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}
