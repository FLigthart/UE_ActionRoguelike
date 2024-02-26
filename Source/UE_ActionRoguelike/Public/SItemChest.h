#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;
class UParticleSystemComponent;

UCLASS()
class UE_ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	float TargetPitch;
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> LidMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> GoldPileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystemComponent> GoldEffect;
	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	ASItemChest();
};
