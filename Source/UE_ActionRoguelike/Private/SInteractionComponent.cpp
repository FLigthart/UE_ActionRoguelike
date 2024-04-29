#include "SInteractionComponent.h"

#include "SGameplayInterface.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "SWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("ar.InteractionDebugDraw"),
	false, TEXT("Enable Debug Lines for Interact Component"), ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 45.0f;
	TraceDistance = 350.f;
	CollisionChannels.Add(ECC_WorldDynamic);
	CollisionChannels.Add(ECC_GameTraceChannel1); //The powerup channel
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Only if we are running the SInteractionComponent, run the interaction code.
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TEnumAsByte<ECollisionChannel> CollisionChannel : CollisionChannels)
	{
		ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	}
}

void USInteractionComponent::PrimaryInteract()
{
	/* Possibly the worst way I could've done this. HealthPotion inherits GameplayInterface and lever doesn't since it's in blueprints.
	If it is the lever, cast will fail, so doesn't have to check if can interact. If it's a HealthPotion, goes inside if statement
	and should check if CanInteract.
	TODO: Implement a single way to handle GameplayInterface. (Find way to inherit GameplayInterface to blueprint, or do it without GameplayInterface).
	*/

	ServerInteract(FocusedActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus)
	{
		ISGameplayInterface* GameplayInterface = Cast<ISGameplayInterface>(FocusedActor);
		if (GameplayInterface && !GameplayInterface->GetCanInteract())
		{
			return;
		}
		
		APawn* MyPawn = Cast<APawn>(GetOwner());
        	
		ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
	}
}

void USInteractionComponent::FindBestInteractable()
{
	AActor* MyOwner = GetOwner();

	UCameraComponent* PlayerCamera = MyOwner->FindComponentByClass<UCameraComponent>();

	if (!ensureAlways(PlayerCamera))
	{
		return;
	}
	
	FVector Start = PlayerCamera->GetComponentLocation();
	FRotator CameraRotation = PlayerCamera->GetComponentRotation();
	FVector End = Start + (CameraRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;
	
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	if (CVarDebugDrawInteraction.GetValueOnGameThread())
	{
		DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0, 2.0f);
	}

	// Clear reference before trying to fill
	FocusedActor = nullptr;
	
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (CVarDebugDrawInteraction.GetValueOnGameThread())
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
			}
			
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	// If the Focused Object exists, and if it is interactable.
	if (FocusedActor)
	{
		// If the GameplayInterface is inherited by object and it can't interact, return.
		ISGameplayInterface* GameplayInterface = Cast<ISGameplayInterface>(FocusedActor);
		if (GameplayInterface && !GameplayInterface->GetCanInteract())
		{
			if (DefaultWidgetInstance)
			{
				DefaultWidgetInstance->RemoveFromParent();
				return;
			}
		}
		
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else if (DefaultWidgetInstance)
	{
		DefaultWidgetInstance->RemoveFromParent();
	}
}
