// Copyright 2025 Team BH.


#include "RsDurationCalculation.h"

#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"
#include "Rs/AbilitySystem/Effect/RsEffectTypes.h"

float URsDurationCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (float SetByCaller_Duration = Spec.GetSetByCallerMagnitude(RsSetByCallerNames::DURATION, false))
	{
		return SetByCaller_Duration;
	}
	if (const FRsEffectTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsEffectTableRow>(Spec))
	{
		return Row->Duration;
	}
	return 0.f;
}
