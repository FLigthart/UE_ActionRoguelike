#include "SAction_ProjectileAttack.h"

#include "SCharInfoInterface.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	AttackDelay = 0.2f;
	
	SpawnSocket = "Muzzle_01";
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	Instigator = Cast<ACharacter>(InstigatorActor);
	
	LoadProjectileClass(true);

	if (!ensure(Instigator))
	{
		return;
	}

	if (Instigator->HasAuthority())
	{
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Instigator);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackDelay, false);
	}
}

void USAction_ProjectileAttack::AttackDelay_Elapsed()
{
	//This is the start location of the actual projectile.
	FVector SpawnLocation = Instigator->GetMesh()->GetSocketLocation(SpawnSocket);

	FTransform SpawnTransform;
	FActorSpawnParameters SpawnParams;
	CalculateSpawnParams(SpawnLocation, &SpawnTransform, &SpawnParams, 5000.f, false);

	if (Projectile)
	{
		GetWorld()->SpawnActor<AActor>(Projectile, SpawnTransform, SpawnParams);
	}

	StopAction(Instigator);
}

void USAction_ProjectileAttack::LoadProjectileClass(bool bShouldLoadAsynchronous) 	// Load ProjectileClass sync
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

	FSoftObjectPath ProjectileClassPath = ProjectileClass.ToSoftObjectPath();

	if (bShouldLoadAsynchronous)
	{
		StreamableHandle = StreamableManager.RequestAsyncLoad(ProjectileClassPath, [this]()
		{
			if (StreamableHandle.IsValid())
			{
				Projectile = Cast<UClass>(StreamableHandle->GetLoadedAsset());

				UE_LOG(LogTemp, Log, TEXT("Finished loading Projectile."));
			
				StreamableHandle->ReleaseHandle();
			}
		});
	}
	else // Load synchronous. This is needed for the BlackholeAttack since it is casted right after the action is started.
	{
		StreamableHandle = StreamableManager.RequestSyncLoad(ProjectileClassPath);

		if (StreamableHandle.IsValid())
		{
			Projectile = Cast<UClass>(StreamableHandle->GetLoadedAsset());

			StreamableHandle->ReleaseHandle();
		}
	}
}

//Calculates the Spawn parameters for a spawn location, given the player is aiming with the middle of the screen (crosshair)
void USAction_ProjectileAttack::CalculateSpawnParams(FVector SpawnLocation, FTransform* SpawnTransform, FActorSpawnParameters* SpawnParams, float LineTraceLength, bool bIsStraight)
{
	ISCharInfoInterface* CameraComp = Cast<ISCharInfoInterface>(Instigator);
	if (!CameraComp)
	{
		return;
	}
	
	//The start location of the line trace is the middle of the screen.
	FTransform Start = CameraComp->GetCameraTransform();

	//Calculate the rotation needed to reach the end location from the hand
	FRotator NewRot = (PerformLineTrace(Start, LineTraceLength, bIsStraight) - SpawnLocation).Rotation();
		
	//The Transform of the projectile spawn.
	*SpawnTransform = FTransform(NewRot, SpawnLocation);
 
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParams.Instigator = Instigator;

	*SpawnParams = spawnParams;
}

FVector USAction_ProjectileAttack::PerformLineTrace(FTransform Start, float LineTraceLength, bool bIsStraight)
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
	Params.AddIgnoredActor(Instigator);
	
	FCollisionObjectQueryParams CollisionChannels;
	CollisionChannels.AddObjectTypesToQuery(ECC_Pawn);
	CollisionChannels.AddObjectTypesToQuery(ECC_WorldStatic);
	CollisionChannels.AddObjectTypesToQuery(ECC_WorldDynamic);
	CollisionChannels.AddObjectTypesToQuery(ECC_PhysicsBody);
	bool bDidHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start.GetLocation(), End, CollisionChannels, Params);

	//DrawDebugLine(GetWorld(), Start.GetLocation(), End, FColor::Red, false, 5.f);
	
	return (bDidHit ? Hit.Location : End);
}


