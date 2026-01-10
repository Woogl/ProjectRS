// Copyright 2025 Team BH.


#include "RsAnimNotifyBase.h"

#include "Rs/Condition/RsCondition.h"

bool URsAnimNotifyBase::PassCondition(AActor* Owner) const
{
	if (!Owner)
	{
		return false;
	}
	if (!Condition)
	{
		// No condition means always pass.
		return true;
	}
	return Condition->IsSatisfied(Owner);
}

bool URsAnimNotifyStateBase::PassCondition(const AActor* Owner) const
{
	if (!Owner)
	{
		return false;
	}
	if (!Condition)
	{
		// No condition means always pass.
		return true;
	}
	return Condition->IsSatisfied(Owner);
}
