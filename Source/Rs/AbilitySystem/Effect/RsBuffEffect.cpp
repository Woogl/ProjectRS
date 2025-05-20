// Copyright 2024 Team BH.


#include "RsBuffEffect.h"

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
