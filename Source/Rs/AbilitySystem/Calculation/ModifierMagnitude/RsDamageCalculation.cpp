// Copyright 2025 Team BH.


#include "RsDamageCalculation.h"

#include "RsCoefficientCalculationBase.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

float URsHealthDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (const FRsDamageTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsDamageTableRow>(Spec))
	{
		FString Expression = Row->HealthDamageExpression;
		return FRsExpressionParser::GetParseResult(Expression, Spec, this);
	}
	UE_LOG(RsLog, Warning, TEXT("Cannot find HealthDamageExpression of [ %s ]"), *Spec.ToSimpleString());
	return 0.f;
}

float URsStaggerDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (const FRsDamageTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsDamageTableRow>(Spec))
	{
		FString Expression = Row->StaggerDamageExpression;
		return FRsExpressionParser::GetParseResult(Expression, Spec, this);
	}
	UE_LOG(RsLog, Warning, TEXT("Cannot find StaggerDamageExpression of [ %s ]"), *Spec.ToSimpleString());
	return 0.f;
}
