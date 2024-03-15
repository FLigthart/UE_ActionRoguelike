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
	
	void Disable();
	void Reactivate();
public:	
	ASPowerupActor();
};
