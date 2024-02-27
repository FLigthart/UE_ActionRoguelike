#include "SDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASDashProjectile::ASDashProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComp->InitialSpeed = 6000.f;
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

