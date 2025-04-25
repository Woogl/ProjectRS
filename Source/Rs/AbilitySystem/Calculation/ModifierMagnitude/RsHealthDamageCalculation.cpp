// Copyright 2024 Team BH.


#include "RsHealthDamageCalculation.h"

#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"
#include "Rs/System/RsGameSetting.h"

float URsHealthDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const float BaseDamage = Super::CalculateBaseMagnitude_Implementation(Spec);

	// Get captured attributes
	const float Defense_Target = FindAttributeMagnitude(RsGameplayTags::TAG_COEFFICIENT_DEFENSE_TARGET, Spec);
	const float CriticalRate_Source = FindAttributeMagnitude(RsGameplayTags::TAG_COEFFICIENT_CRITICALRATE_SOURCE, Spec);
	const float CriticalBonus_Source = FindAttributeMagnitude(RsGameplayTags::TAG_COEFFICIENT_CRITICALBONUS_SOURCE, Spec);

	// Check critical hit
	// NOTE: Not guaranteed to be consistent in multiplayer
	const float RandomValue = FMath::RandRange(0.f, 100.f);
	const bool bCriticalHit = CriticalRate_Source >= RandomValue;
	if (bCriticalHit)
	{
		FRsGameplayEffectContext* ContextHandle = static_cast<FRsGameplayEffectContext*>(Spec.GetContext().Get());
		ContextHandle->bIsCriticalHit = true;
	}

	float FinalDamage = BaseDamage;
	// Critical calc
	FinalDamage *= (bCriticalHit ? (1.f + CriticalBonus_Source * 0.01f) : 1.f);
	// Defense rate calc
	FinalDamage *= (190.f / (Defense_Target + 190.f));
	
	if (FinalDamage <= 0.f)
	{
		return 0.f;
	}

	// Groggy has 160 % damage bonus
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	if (TargetTags->HasTagExact(URsGameSetting::Get()->GroggyAbilityTag))
	{
		FinalDamage *= 1.6f;
	}

	return FinalDamage;
}
