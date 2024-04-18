#include "SPlayerState.h"

ASPlayerState::ASPlayerState()
{
	CreditAmount = 0;
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