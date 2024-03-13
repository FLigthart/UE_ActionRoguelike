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

	UPROPERTY(EditAnywhere, Category = "Attack|Projectile")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack|Projectile")
	TObjectPtr<UParticleSystem> CastEffect;
	
	UPROPERTY(EditAnywhere, Category = "Attack|BlackHole")
	TSubclassOf<AActor> BlackHoleClass;

	UPROPERTY(EditAnywhere, Category = "Attack|Dash")
	TSubclassOf<AActor> DashClass;

	UPROPERTY(EditAnywhere, Category = "Attack|Projectile")
	TObjectPtr<UAnimMontage> AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport|Dash")
	TObjectPtr<UAnimMontage> TeleportMontage;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_DashAbility;

	float PrimaryAttackDelay;
	
	float DashAbilityDelay;
	float DashAbilityExitDelay;

	UPROPERTY(EditDefaultsOnly)
	FName HandSocket;
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

	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USInteractionComponent> InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAttributeComponent> AttributeComp;
	
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	//Movement
	void Move(const FInputActionValue& InputValue);
	
	void LookMouse(const FInputActionValue& InputValue);

	//This was for the ExplosiveBarrel. Might fix later
	//UFUNCTION()
	//virtual void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser ) override;

	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	
	void CalculateSpawnParams(FVector SpawnLocation, FTransform* SpawnTransform, FActorSpawnParameters* SpawnTM, float LineTraceLength, bool bIsStraight);
	FVector PerformLineTrace(FTransform Start, float LineTraceLength, bool bIsStraight); //bIsStraight determines if the LineTrace should keep the Z location of the Start vector.

	void PrimaryInteract();

	void BlackHoleAttack();

	void DashAbility(); //Gets called when the dash ability is used
	void DashAbility_TimeElapsed();
	void DashAbility_Exit();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
