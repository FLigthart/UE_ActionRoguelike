#include "SItemChest.h"

#include "Particles/ParticleSystemComponent.h"

ASItemChest::ASItemChest()
{

	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	GoldPileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GoldPileMesh"));
	GoldPileMesh->SetupAttachment(BaseMesh);

	GoldEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GoldEffect"));
	GoldEffect->SetupAttachment(GoldPileMesh);
	GoldEffect->SetAutoActivate(false);

	TargetPitch = 110;
}


void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}