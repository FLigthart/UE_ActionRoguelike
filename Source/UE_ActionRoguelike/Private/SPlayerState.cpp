#include "SPlayerState.h"

#include "Net/UnrealNetwork.h"

ASPlayerState::ASPlayerState()
{
	CreditAmount = 0;
}

void ASPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, CreditAmount,CreditAmount - OldCredits);
}

bool ASPlayerState::AddCredits(int32 Delta)
{
	if (Delta < 0)
	{
		return false;
	}
	
	CreditAmount += Delta;

	OnCreditsChanged.Broadcast(this, CreditAmount, Delta);
	
	return true;
}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
	if (Delta < 0)
	{
		return false;
	}

	if (CreditAmount < Delta)
	{
		return false;
	}

	CreditAmount -= Delta;

	OnCreditsChanged.Broadcast(this, CreditAmount, -Delta);
	
	return true;
}

int32 ASPlayerState::GetCreditAmount() const
{
	return CreditAmount;
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, CreditAmount);
}
