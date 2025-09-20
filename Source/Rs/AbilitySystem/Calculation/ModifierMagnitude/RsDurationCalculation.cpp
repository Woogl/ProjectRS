// Copyright 2024 Team BH.


#include "RsDurationCalculation.h"

#include "Rs/RsGameplayTags.h"

float URsDurationCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	return Spec.GetSetByCallerMagnitude(RsGameplayTags::DURATION, true);
}
