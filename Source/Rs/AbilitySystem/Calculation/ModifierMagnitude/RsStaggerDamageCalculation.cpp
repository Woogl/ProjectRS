// Copyright 2024 Team BH.


#include "RsStaggerDamageCalculation.h"

float URsStaggerDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const float BaseDamage = Super::CalculateBaseMagnitude_Implementation(Spec);

	// Stagger damage calculation
	if (BaseDamage <= 0.f)
	{
		return 0.f;
	}

	return BaseDamage;
}
