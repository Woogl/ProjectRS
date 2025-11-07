// Copyright 2025 Team BH.


#include "RsStaggerDamageCalculation.h"

#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

float URsStaggerDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (const FRsDamageTableRow* Row = URsAbilitySystemGlobals::FindTableRowFromSpec<FRsDamageTableRow>(Spec))
	{
		FString StaggerDamageExpression = Row->StaggerDamageExpression;
		if (StaggerDamageExpression.IsNumeric())
		{
			return FCString::Atof(*StaggerDamageExpression);
		}
		else
		{
			// TODO: Implement
		}
	}
	return 0.f;
}
