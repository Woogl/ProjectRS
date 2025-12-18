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
		const FString Script = Row->HealthDamageExpression;
		return FRsCoefficientScriptParser::GetParseResult(Script, Spec, this);
	}
	UE_LOG(RsAbilityLog, Warning, TEXT("Cannot find HealthDamageExpression of [%s]"), *Spec.ToSimpleString());
	return 0.f;
}

float URsStaggerDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (const FRsDamageTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsDamageTableRow>(Spec))
	{
		const FString Script = Row->StaggerDamageExpression;
		return FRsCoefficientScriptParser::GetParseResult(Script, Spec, this);
	}
	UE_LOG(RsAbilityLog, Warning, TEXT("Cannot find StaggerDamageExpression of [%s]"), *Spec.ToSimpleString());
	return 0.f;
}
