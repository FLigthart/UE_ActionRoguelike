#include "UE_ActionRoguelike/Public/SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASCharacter::ASCharacter()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";
	HitFleshColorParamName = "HitFleshColor";
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
	//OnTakeAnyDamage.AddDynamic(this, &ASCharacter::TakeDamage); //For the explosive barrel
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	if (Delta <= 0) //Damage, red effect
	{
		GetMesh()->SetVectorParameterValueOnMaterials(HitFleshColorParamName, FVector(1, 0, 0));
	}
	else //Healing, green effect
	{
		GetMesh()->SetVectorParameterValueOnMaterials(HitFleshColorParamName, FVector(0, 1, 0));
	}
	
	if (NewHealth <= 0.0f && Delta < 0.0f) //Die
	{
		DisableInput(Cast<APlayerController>(GetController()));
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const TObjectPtr<APlayerController> PC = GetController<APlayerController>();
	const TObjectPtr<ULocalPlayer> LP = PC->GetLocalPlayer();

	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->AddMappingContext(DefaultInputMapping, 0);
	
	// Convert it to the Enhanced Input system
	TObjectPtr<UEnhancedInputComponent> InputComp = Cast <UEnhancedInputComponent>(PlayerInputComponent);

	//Basic Bindings
	InputComp->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ASCharacter::Move);
	InputComp->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ASCharacter::LookMouse);
	InputComp->BindAction(Input_Jump, ETriggerEvent::Started, this, &ASCharacter::Jump);
	InputComp->BindAction(Input_PrimaryAttack, ETriggerEvent::Started, this,  &ASCharacter::PrimaryAttack);
	InputComp->BindAction(Input_PrimaryInteract, ETriggerEvent::Started, this,  &ASCharacter::PrimaryInteract);
	InputComp->BindAction(Input_BlackHole, ETriggerEvent::Started, this,  &ASCharacter::BlackHoleAttack);
	InputComp->BindAction(Input_Dash, ETriggerEvent::Started, this,  &ASCharacter::DashAbility);
	InputComp->BindAction(Input_Sprint, ETriggerEvent::Started, this,  &ASCharacter::SprintStart);
	InputComp->BindAction(Input_Sprint, ETriggerEvent::Completed, this,  &ASCharacter::SprintStop);
}

/*
 *	SCharInfoInterface Functions
 */

FTransform ASCharacter::GetCameraTransform() const
{
	if (ensure(CameraComp))
	{
		return FTransform(CameraComp->GetComponentRotation(), CameraComp->GetComponentLocation());
	}
	return FTransform(FRotator::ZeroRotator, FVector::ZeroVector);
}

/*
 *	Console Commands
 */

void ASCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

/*
 * End of Console Commands
 */

void ASCharacter::Move(const FInputActionValue& InputValue)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//X = Forward (Red)
	//Y = Right (Green)
	//Z = Up (Blue)
	
	//Get value from input (combined value from WASD keys or single Gamepad stick) and convert it to Vector2d (x,y)
	const FVector2D AxisValue = InputValue.Get<FVector2d>();

	//Move forward/backwards
	AddMovementInput(ControlRot.Vector(), AxisValue.Y);

	//Move Right/Left strafe
	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, AxisValue.X);
}

void ASCharacter::LookMouse(const FInputActionValue& InputValue)
{
	const FVector2d Value = InputValue.Get<FVector2d>();

	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

//TODO: Implement a proper way to take damage if the course doesn't do that. See that event in PostIni and SExplosiveBarrel
/*void ASCharacter::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser )
{
	UE_LOG(LogTemp, Log, TEXT("Receive damage event triggered. Damage amount: %f"), Damage);
	float ActualDamage = Super::TakeDamage(Damage, UDamageType::StaticClass(), InstigatedBy, DamageCauser);
	AttributeComp->ApplyHealthChange(ActualDamage);
}*/

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::BlackHoleAttack() //Implementation of BlackHole is in Blueprints
{
	ActionComp->StartActionByName(this, "BlackHoleAttack");
}

//This is only for calculating the path, animations on character and spawning the DashProjectile. The usage is implemented in SDashProjectile
void ASCharacter::DashAbility()
{
	ActionComp->StartActionByName(this, "Dash");
}
