#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class UParticleEmitter;
class UCameraComponent;
class USpringArmComponent;

class UInputMappingContext;
class UInputAction;
class USInteractionComponent;
class UAnimMontage;
struct FInputActionValue;


UCLASS()
class UE_ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

	FTimerHandle TimerHandle_DashExplode;
	FTimerHandle TimerHandle_DashTeleport;

	float PrimaryAttackDelay;

	float DetonateDelay = 0.2f;
	float TeleportDelay = 0.2f;
	
public:
	ASCharacter();

protected:

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Move;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Look;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Jump;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_PrimaryInteract;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_BlackHole;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Dash;

	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USInteractionComponent> InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAttributeComponent> AttributeComp;
	
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	//Movement
	void Move(const FInputActionValue& InputValue);
	
	void LookMouse(const FInputActionValue& InputValue);

	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	
	void CalculateSpawnParams(FVector SpawnLocation, FTransform* SpawnTransform, FActorSpawnParameters* SpawnTM, float LineTraceLength, bool bIsStraight);
	FVector PerformLineTrace(FTransform Start, float LineTraceLength, bool bIsStraight); //bIsStraight determines if the LineTrace should keep the Z location of the Start vector.

	void PrimaryInteract();

	void BlackHoleAttack();

	void DashAbility(); //Gets called when the ability is used
	void DashAbility_Detonation(); //Gets called after 0.2s after the ability is used. Ability Explodes.
	void DashAbility_Dash(); //After another 0.2s the instigator teleports to the location of the projectile.

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
