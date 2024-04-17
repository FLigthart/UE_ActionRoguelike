#include "SAction.h"

#include "SActionComponent.h"

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning() || GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	//If bIsRunning is false, but we still call StopAction, we did something wrong.
	ensureAlways(bIsRunning);
	
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}

UWorld* USAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}
