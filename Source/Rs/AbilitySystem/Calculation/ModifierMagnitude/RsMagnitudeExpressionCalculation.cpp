// Copyright 2025 Team BH.


#include "RsMagnitudeExpressionCalculation.h"

#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

float URsMagnitudeExpressionCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (const FRsEffectTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsEffectTableRow>(Spec))
	{
		FString Expression = Row->FindValue<FString>(TEXT("MagnitudeExpression"), true);
		return FRsExpressionParser::GetParseResult(Expression, Spec, this);;
	}
	UE_LOG(RsLog, Warning, TEXT("Cannot find magnitude expression of [ %s ]"), *Spec.ToSimpleString());
	return 0.f;
}
