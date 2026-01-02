// Copyright 2025 Team BH.


#include "RsDurationCalculation.h"

#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

float URsDurationCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (const FRsEffectTableRow* Row = URsAbilitySystemGlobals::GetEffectTableRow<FRsEffectTableRow>(Spec.GetContext()))
	{
		return Row->Duration;
	}
	UE_LOG(LogRsAbility, Warning, TEXT("Can't find effect duration : %s"), *Spec.ToSimpleString());
	return 0.f;
}
