// Copyright 2024 Team BH.


#include "RsDamageEffect.h"

#include "Rs/AbilitySystem/EffectComponent/RsAdditionalDamageEffectGameplayEffectComponent.h"

void URsDamageEffect::PostInitProperties()
{
	Super::PostInitProperties();
	AddComponent<URsAdditionalDamageEffectGameplayEffectComponent>();
	CachedAssetTags.AppendTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Effect.Damage")));
}
