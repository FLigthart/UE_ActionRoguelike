#pragma once

#include "CoreMinimal.h"
#include "BaseAttack.h"
#include "GameFramework/Actor.h"
#include "SDashProjectile.generated.h"

UCLASS()
class UE_ACTIONROGUELIKE_API ASDashProjectile : public ABaseAttack
{
	GENERATED_BODY()
	
public:	
	ASDashProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
