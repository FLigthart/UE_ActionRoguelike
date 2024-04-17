#pragma once

#include "CoreMinimal.h"
#include "SCharInfoInterface.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USActionComponent;
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
class UE_ACTIONROGUELIKE_API ASCharacter : public ACharacter, public ISCharInfoInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Parameter Names")
	FName HitFleshColorParamName;
	
	UPROPERTY(VisibleAnywhere, Category = "Parameter Names")
	FName TimeToHitParamName;
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

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Sprint;

	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USInteractionComponent> InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAttributeComponent> AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USActionComponent> ActionComp;
	
	virtual void PostInitializeComponents() override;

	/*
	 *	Movement
	 */
	void Move(const FInputActionValue& InputValue);
	
	void LookMouse(const FInputActionValue& InputValue);

	void SprintStart();
	void SprintStop();

	//This was for the ExplosiveBarrel. Might fix later
	//UFUNCTION()
	//virtual void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser ) override;

	/*
	 *	Attacks
	 */
	
	void PrimaryAttack();

	void PrimaryInteract();

	void BlackHoleAttack();

	void DashAbility(); //Is called when the dash ability is used
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	 *	SCharInfoInterface Functions
	 */

	virtual FTransform GetCameraTransform() const override;
	
	/*
	 *	Console Commands
	 */
	
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);
};
