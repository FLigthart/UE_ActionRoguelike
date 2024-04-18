#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

class USphereComponent;

UCLASS()
class UE_ACTIONROGUELIKE_API ASPowerupActor : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
 
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float RespawnTime;

	//Spawn rate in the case that it is spawned on game start.
	UPROPERTY(EditDefaultsOnly, Category = "Credits", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float PowerupSpawnRate;
	
	void Disable();
	void Reactivate();
	
public:	
	ASPowerupActor();

	float GetPowerupSpawnRate();
};
