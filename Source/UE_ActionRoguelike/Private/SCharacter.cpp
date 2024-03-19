#include "UE_ActionRoguelike/Public/SCharacter.h"

#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
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
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;

	PrimaryAttackDelay = 0.2f;
	DashAbilityDelay = 0.44f; //About the length of the intro animation.
	DashAbilityExitDelay = 0.2f; //Time from spawn till teleportation

	TimeToHitParamName = "TimeToHit";
	HitFleshColorParamName = "HitFleshColor";
	
	HandSocket = "Muzzle_01";
	SpineSocket = "spine_Socket";
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
	
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		DisableInput(Cast<APlayerController>(GetController()));
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
}

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

//TODO: Implement a proper way to take damage if the course doesn't do that. See that event in PostIni and SExplosiveBarrel
/*void ASCharacter::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser )
{
	UE_LOG(LogTemp, Log, TEXT("Receive damage event triggered. Damage amount: %f"), Damage);
	float ActualDamage = Super::TakeDamage(Damage, UDamageType::StaticClass(), InstigatedBy, DamageCauser);
	AttributeComp->ApplyHealthChange(ActualDamage);
}*/

void ASCharacter::PrimaryAttack()
{
	if (bIsPrimaryAttacking) //Return if already attacking to prevent attack spamming.
	{
		return;
	}

	bIsPrimaryAttacking = true;
	
	PlayAnimMontage(AttackAnim);

	UGameplayStatics::SpawnEmitterAttached(CastEffect, GetMesh(), HandSocket, FVector::ZeroVector,
		FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
	
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, PrimaryAttackDelay);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	if (ensureAlways(ProjectileClass))
	{
		//This is the start location of the actual projectile.
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocket);

		FTransform SpawnTransform;
		FActorSpawnParameters SpawnParams;
		CalculateSpawnParams(HandLocation, &SpawnTransform, &SpawnParams, 5000.f, false);
		
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
	}
	
	bIsPrimaryAttacking = false;
}

//Calculates the Spawn parameters for a spawn location, given the player is aiming with the middle of the screen (crosshair)
void ASCharacter::CalculateSpawnParams(FVector SpawnLocation, FTransform* SpawnTransform, FActorSpawnParameters* SpawnParams, float LineTraceLength, bool bIsStraight)
{
	//The start location of the line trace is the middle of the screen.
	FTransform Start = FTransform(CameraComp->GetComponentRotation(), CameraComp->GetComponentLocation());

	//Calculate the rotation needed to reach the end location from the hand
	FRotator NewRot = (PerformLineTrace(Start, LineTraceLength, bIsStraight) - SpawnLocation).Rotation();
		
	//The Transform of the projectile spawn.
	*SpawnTransform = FTransform(NewRot, SpawnLocation);
 
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParams.Instigator = this;

	*SpawnParams = spawnParams;
}

//Perform line trace to middle of screen.
FVector ASCharacter::PerformLineTrace(FTransform Start, float LineTraceLength, bool bIsStraight)
{
	FHitResult Hit;
	FVector End;
	if (!bIsStraight)
	{
		End = Start.GetLocation() + (Start.Rotator().Vector() * LineTraceLength);
	}
	else
	{
		FVector TempVector = Start.Rotator().Vector();
		TempVector.Z = 0.f;
		End = Start.GetLocation() + (TempVector * LineTraceLength);
	}

	//Ignore the Instigator
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	FCollisionObjectQueryParams CollisionChannels;
	CollisionChannels.AddObjectTypesToQuery(ECC_Pawn);
	CollisionChannels.AddObjectTypesToQuery(ECC_WorldStatic);
	CollisionChannels.AddObjectTypesToQuery(ECC_WorldDynamic);
	CollisionChannels.AddObjectTypesToQuery(ECC_PhysicsBody);
	bool bDidHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start.GetLocation(), End, CollisionChannels, Params);

	//DrawDebugLine(GetWorld(), Start.GetLocation(), End, FColor::Red, false, 5.f);
	
	return (bDidHit ? Hit.Location : End);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

//TODO: Refactor BlackHoleAttack(), DashAbility() and PrimaryAttack_TimeElapsed() into one function with the class and socket as parameters.
void ASCharacter::BlackHoleAttack() //Implementation is in Blueprints
{
	if (ensureAlways(BlackHoleClass))
	{
		//This is the start location of the actual projectile.
		FVector SpawnLocation = GetMesh()->GetSocketLocation(SpineSocket); //Spawn about 40 cm from chest

		FTransform SpawnTransform;
		FActorSpawnParameters SpawnParams;
		CalculateSpawnParams(SpawnLocation, &SpawnTransform, &SpawnParams, 5000.f, true);
		
		GetWorld()->SpawnActor<AActor>(BlackHoleClass, SpawnTransform, SpawnParams);
	}
}

//This is only for calculating the path, animations on character and spawning the DashProjectile. The usage is implemented in SDashProjectile
void ASCharacter::DashAbility()
{
	if (ensureAlways(DashClass))
	{
		UGameplayStatics::SpawnEmitterAttached(CastEffect, GetMesh(), HandSocket, FVector::ZeroVector,
	FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		
		PlayAnimMontage(TeleportMontage, 1, "IntroStart");

		
		GetWorldTimerManager().SetTimer(TimerHandle_DashAbility, this, &ASCharacter::DashAbility_TimeElapsed, DashAbilityDelay);
	}
}

void ASCharacter::DashAbility_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation(HandSocket);
 
 	FTransform SpawnTransform;
 	FActorSpawnParameters SpawnParams;
 	CalculateSpawnParams(HandLocation, &SpawnTransform, &SpawnParams, 5000.f,false);
 	
 	GetWorld()->SpawnActor<AActor>(DashClass, SpawnTransform, SpawnParams);
	
	GetWorldTimerManager().SetTimer(TimerHandle_DashAbility, this, &ASCharacter::DashAbility_Exit, DashAbilityExitDelay);
}

void ASCharacter::DashAbility_Exit()
{
	PlayAnimMontage(TeleportMontage, 1, "EndStart");
}
