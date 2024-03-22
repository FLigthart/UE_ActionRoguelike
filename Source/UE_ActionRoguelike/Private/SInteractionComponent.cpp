#include "SInteractionComponent.h"

#include "SGameplayInterface.h"
#include "Camera/CameraComponent.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("ar.InteractionDebugDraw"),
	false, TEXT("Enable Debug Lines for Interact Component"), ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1); //The powerup channel

	AActor* MyOwner = GetOwner();

	UCameraComponent* PlayerCamera = MyOwner->FindComponentByClass<UCameraComponent>();

	if (ensureAlways(PlayerCamera))
	{
		FVector Start = PlayerCamera->GetComponentLocation();
		FRotator CameraRotation = PlayerCamera->GetComponentRotation();
		FVector End = Start + (CameraRotation.Vector() * 350.f);
	
		TArray<FHitResult> Hits;

		float Radius = 45.f;
		FCollisionShape Shape;
		Shape.SetSphere(Radius);
	
		bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);
	
		FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

		if (CVarDebugDrawInteraction.GetValueOnGameThread())
		{
			DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0, 2.0f);
		}
		
		for (FHitResult Hit : Hits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				if (CVarDebugDrawInteraction.GetValueOnGameThread())
				{
					DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
				}
					
				ISGameplayInterface* GameplayInterface = Cast<ISGameplayInterface>(HitActor);
				if (HitActor->Implements<USGameplayInterface>())
				{
					/* Possibly the worst way I could've done this. HealthPotion inherits GameplayInterface and lever doesn't since it's in blueprints.
					If it is the lever, cast will fail, so doesn't have to check if can interact. If it's a HealthPotion, goes inside if statement
					and should check if CanInteract.
					TODO: Implement a single way to handle GameplayInterface. (Find way to inherit GameplayInterface to blueprint, or do it without GameplayInterface).
					*/
					if (GameplayInterface)
					{
						if (GameplayInterface->GetCanInteract())
						{
							APawn* MyPawn = Cast<APawn>(MyOwner);
        		
							ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
							break;
						}
					}
					else
					{
						APawn* MyPawn = Cast<APawn>(MyOwner);
        		
						ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
						break;
					}
				}
			}
		}
	}
}
