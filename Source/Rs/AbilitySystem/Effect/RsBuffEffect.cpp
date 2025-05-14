// Copyright 2024 Team BH.


#include "RsBuffEffect.h"

#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/Calculation/ModifierMagnitude/RsDurationCalculation.h"

URsBuffEffect::URsBuffEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	
	// FCustomCalculationBasedFloat CustomDuration;
	// CustomDuration.CalculationClassMagnitude = URsDurationCalculation::StaticClass();
	// DurationMagnitude = FGameplayEffectModifierMagnitude(CustomDuration);
	//
	// FGameplayModifierInfo ModifierInfo;
	// // Set Attribute in BP
	// FCustomCalculationBasedFloat CustomMagnitude;
	// CustomMagnitude.CalculationClassMagnitude = URsCoefficientCalculation::StaticClass();
	// Modifiers.Add(ModifierInfo);
}

FRsEffectCoefficient URsBuffEffect::ToRsEffectCoefficient(const FRsBuffCoefficient& BuffCoefficient, float Duration)
{
	return BuffCoefficient.ToRsEffectCoefficient(Duration);
}

FRsEffectCoefficient FRsBuffCoefficient::ToRsEffectCoefficient(float Duration) const
{
	FRsEffectCoefficient EffectCoefficient = FRsEffectCoefficient(BuffClass, Coefficients);
	EffectCoefficient.Coefficients.Add(RsGameplayTags::MANUAL_DURATION, Duration);
	return EffectCoefficient;
}
