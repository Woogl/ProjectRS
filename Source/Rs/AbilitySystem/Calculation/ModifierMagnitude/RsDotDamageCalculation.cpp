// Copyright 2024 Team BH.


#include "RsDotDamageCalculation.h"

float URsDotDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float TotalDamage = Super::CalculateBaseMagnitude_Implementation(Spec);
	float TickDamage = TotalDamage * Spec.GetPeriod() / Spec.GetDuration();
	return TickDamage;
}