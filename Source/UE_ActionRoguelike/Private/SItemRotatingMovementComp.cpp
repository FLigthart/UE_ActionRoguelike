#include "SItemRotatingMovementComp.h"

USItemRotatingMovementComp::USItemRotatingMovementComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	YawRate = 50.f;
	RotationRate = FRotator(0.0f, YawRate, 0.0f);

	bShouldMove = true;
	MovementSpeed = 0.25f;
	MovementLength = 1.5f;
}

void USItemRotatingMovementComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bShouldMove && GetOwner())
	{
		float Speed = GetWorld()->GetTimeSeconds() * MovementLength;
		FVector ActorLocation = GetOwner()->GetActorLocation();
		FVector NewActorLocation = FVector(ActorLocation.X, ActorLocation.Y, ActorLocation.Z + (MovementSpeed * FMath::Sin(Speed)));
		GetOwner()->SetActorLocation(NewActorLocation);
	}
}
