#include "SPowerupActor.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
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

	SpawnHeightOffset = 0.0f;

	bIsActive = true;
	
	bReplicates = true;
}

void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + SpawnHeightOffset));
}

float ASPowerupActor::GetPowerupSpawnRate()
{
	return PowerupSpawnRate;
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	//Setup logic in derived classes
}

FText ASPowerupActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ASPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);

	RootComponent->SetVisibility(bIsActive, true);
}

void ASPowerupActor::Disable()
{
	bIsActive = false;
	
	OnRep_IsActive();
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASPowerupActor::Reactivate, RespawnTime, false);
}

void ASPowerupActor::Reactivate()
{
	bIsActive = true;
	
	OnRep_IsActive();
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerupActor, bIsActive);
}


