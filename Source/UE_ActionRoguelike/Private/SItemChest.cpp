#include "SItemChest.h"

#include "SActionComponent.h"
#include "SCharacter.h"
#include "Net/UnrealNetwork.h"
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

	bReplicates = true;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void ASItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpened);
}

void ASItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

FText ASItemChest::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	// TODO: If GameplayTags to open chests are reimplemented, add text to display which tag is needed here.
	return FText::GetEmpty();
}
