// Copyright 2025 Team BH.


#include "RsHealthDamageCalculation.h"

#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

float URsHealthDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (const FRsDamageTableRow* Row = URsAbilitySystemGlobals::FindTableRowFromSpec<FRsDamageTableRow>(Spec))
	{
		FString HealthDamageExpression = Row->HealthDamageExpression;
		if (HealthDamageExpression.IsNumeric())
		{
			return FCString::Atof(*HealthDamageExpression);
		}
		else
		{
			// TODO: Implement
		}
	}
	return 0.f;
}
