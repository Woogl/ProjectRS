// Copyright 2025 Team BH.


#include "RsDamageCalculation.h"

#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Calculation/RsParser.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

float URsHealthDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (const FRsDamageTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsDamageTableRow>(Spec))
	{
		const FString Script = Row->HealthDamageExpression;
		return FRsParser::CoefficientScriptToFloat(Script, Spec);
	}
	UE_LOG(LogRsAbility, Warning, TEXT("Cannot find HealthDamageExpression of [%s]"), *Spec.ToSimpleString());
	return 0.f;
}

float URsStaggerDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (const FRsDamageTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsDamageTableRow>(Spec))
	{
		const FString Script = Row->StaggerDamageExpression;
		return FRsParser::CoefficientScriptToFloat(Script, Spec);
	}
	UE_LOG(LogRsAbility, Warning, TEXT("Cannot find StaggerDamageExpression of [%s]"), *Spec.ToSimpleString());
	return 0.f;
}
