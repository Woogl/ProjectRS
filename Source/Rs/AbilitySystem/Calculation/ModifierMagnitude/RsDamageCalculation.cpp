// Copyright 2025 Team BH.


#include "RsDamageCalculation.h"

#include "RsCoefficientCalculation.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

float URsHealthDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (const FRsDamageTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsDamageTableRow>(Spec))
	{
		FString Expression = Row->HealthDamageExpression;
		return FRsEffectMagnitudeExpressionParser::GetParseResult(Expression, Spec, this);
	}
	return 0.f;
}

float URsStaggerDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (const FRsDamageTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsDamageTableRow>(Spec))
	{
		FString Expression = Row->StaggerDamageExpression;
		return FRsEffectMagnitudeExpressionParser::GetParseResult(Expression, Spec, this);
	}
	return 0.f;
}
