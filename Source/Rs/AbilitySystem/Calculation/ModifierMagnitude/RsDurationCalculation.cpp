// Copyright 2024 Team BH.


#include "RsDurationCalculation.h"

#include "Rs/System/RsDeveloperSetting.h"

float URsDurationCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	return Spec.GetSetByCallerMagnitude(URsDeveloperSetting::Get()->ManualDurationTag, true);
}
