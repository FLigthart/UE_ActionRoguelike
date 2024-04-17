#include "SAction_BlackHoleAttack.h"

#include "GameFramework/Character.h"


USAction_BlackHoleAttack::USAction_BlackHoleAttack()
{
	SpawnSocket = "spine_Socket";
}

void USAction_BlackHoleAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	USAction::StartAction_Implementation(InstigatorActor);
	
	Instigator = Cast<ACharacter>(InstigatorActor);
	
	if (ensure(Instigator && ProjectileClass))
	{
		FVector SpawnLocation = Instigator->GetMesh()->GetSocketLocation(SpawnSocket); //Spawn about 40 cm from chest

		FTransform SpawnTransform;
		FActorSpawnParameters SpawnParams;
		CalculateSpawnParams(SpawnLocation, &SpawnTransform, &SpawnParams, 5000.f, true);
		
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);

		StopAction(Instigator);
	}

}
