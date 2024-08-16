#include "SActionEffect.h"

#include "SActionComponent.h"
#include "GameFramework/GameStateBase.h"


USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

float USActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>();

	if (GameState)
	{
		float EndTime = TimeStarted + Duration;	// World time at which the ActionEffect will expire.
		return EndTime - GameState->GetServerWorldTimeSeconds();
	}

	return Duration;	// If no GameState available, return duration since that usually happens at the beginning of the game.
}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	// If the Period Timer is almost over, we don't want to lose that last tick. So execute it before action is stopped.
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	USActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	
}
