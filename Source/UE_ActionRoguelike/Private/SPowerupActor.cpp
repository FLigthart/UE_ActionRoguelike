#include "SPowerupActor.h"

#include "Components/SphereComponent.h"
#include "PhysicsEngine/ShapeElem.h"

ASPowerupActor::ASPowerupActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(40.f);
	SphereComp->SetCollisionProfileName("Powerup");
	SphereComp->SetupAttachment(RootComponent);

	RespawnTime = 10.f;

	PowerupSpawnRate = 0.025f;
}

float ASPowerupActor::GetPowerupSpawnRate()
{
	return PowerupSpawnRate;
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	//Setup logic in derived classes
}

void ASPowerupActor::Disable()
{
	MeshComp->SetVisibility(false, true);
	
	SetCanInteract(false);
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASPowerupActor::Reactivate, RespawnTime, false);
}

void ASPowerupActor::Reactivate()
{
	MeshComp->SetVisibility(true, true);

	SetCanInteract(true);
}


