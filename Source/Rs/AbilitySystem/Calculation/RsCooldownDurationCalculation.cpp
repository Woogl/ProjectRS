// Copyright 2024 Team BH.


#include "RsCooldownDurationCalculation.h"

#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"

float URsCooldownDurationCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const URsGameplayAbility* Ability = Cast<URsGameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());

	if (!Ability)
	{
		return 0.f;
	}

	return Ability->CooldownDuration.GetValueAtLevel(Ability->GetAbilityLevel());
}
