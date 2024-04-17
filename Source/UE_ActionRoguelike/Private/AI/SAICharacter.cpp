#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	TimeToHitParamName = "TimeToHit";
	HitFleshColorParamName = "HitFleshColor";
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	
	if (Delta <= 0.0f) 
	{

		if (!InstigatorActor->IsA(ASAICharacter::StaticClass())) //Do not target lock when another SAICharacter hits the SAICharacter
		{
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr && NewHealth > 0.0f) //Assign an active HealthBar, if not yet assigned. Don't assign if its an instant kill.
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		//Damage. Red Material Effect.
		GetMesh()->SetVectorParameterValueOnMaterials(HitFleshColorParamName, FVector(1, 0, 0));
		
		if (NewHealth <= 0.0f) //Dead
		{
			//Stop Behaviour Tree
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->BrainComponent->StopLogic("Died");
			}

			//Ragdoll
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			SetLifeSpan(10.0f);
		}
	}
	
	else //Healing, green effect
	{
		GetMesh()->SetVectorParameterValueOnMaterials(HitFleshColorParamName, FVector(0, 1, 0));
	}
}
