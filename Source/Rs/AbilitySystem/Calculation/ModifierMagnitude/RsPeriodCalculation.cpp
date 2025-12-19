// Copyright 2025 Team BH.


#include "RsPeriodCalculation.h"

#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

float URsPeriodCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (const FRsEffectTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsEffectTableRow>(Spec))
	{
		return Row->FindValue<float>(TEXT("Period"), true);
	}
	UE_LOG(RsAbilityLog, Warning, TEXT("Cannot find HealthDamageExpression of [%s]"), *Spec.ToSimpleString());
	return 0.f;
}
