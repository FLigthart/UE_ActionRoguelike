#include "SExplosiveBarrel.h"

#include "SAttributeComponent.h"
#include "SMagicProjectile.h"
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
		Explode();

		USAttributeComponent* InteractionComp = OtherActor->GetInstigator()->GetComponentByClass<USAttributeComponent>();
		if (InteractionComp)
		{
			InteractionComp->ApplyHealthChange(DamageAmount);
		}
	}
}

void ASExplosiveBarrel::Explode()
{
	RadialForce->FireImpulse();
}
