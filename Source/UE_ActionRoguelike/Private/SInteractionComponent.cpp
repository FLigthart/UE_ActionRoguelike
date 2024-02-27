#include "SInteractionComponent.h"

#include "SGameplayInterface.h"
#include "Camera/CameraComponent.h"

USInteractionComponent::USInteractionComponent()
{
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

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
	
		for (FHitResult Hit : Hits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				if(HitActor->Implements<USGameplayInterface>())
				{
					APawn* MyPawn = Cast<APawn>(MyOwner);
        		
					ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
					DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
					break;
				}
			}
		}
	}
}
