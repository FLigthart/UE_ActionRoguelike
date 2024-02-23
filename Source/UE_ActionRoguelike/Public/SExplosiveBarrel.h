#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class URadialForceComponent;

UCLASS()
class UE_ACTIONROGUELIKE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	ASExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<URadialForceComponent> RadialForce;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	virtual void PostInitializeComponents() override; 

public:

};
