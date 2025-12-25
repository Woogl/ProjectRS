// Copyright 2025 Team BH.


#include "RsAnimNotifyState_TimeControl.h"

#include "AbilitySystemComponent.h"
#include "Rs/World/TimeControl/RsTimeControlLibrary.h"

void URsAnimNotifyState_TimeControl::HandleConditionSatisfied()
{
	if (UAbilitySystemComponent* ASC = OwnerASC.Get())
	{
		URsTimeControlLibrary::RequestTimeDilation(ASC, GetFNameSafe(this), ERsTimeControlPriority::VFX, TimeDilation, CachedTotalDuration, BlendTime);
	}

	Super::HandleConditionSatisfied();
}
