// Copyright 2025 Team BH.


#include "RsDamageEffectComponent.h"

#include "GameplayEffect.h"
#include "RsCanApplyEffectComponent.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"

void URsDamageEffectComponent::OnGameplayEffectChanged()
{
	Super::OnGameplayEffectChanged();
	
	UGameplayEffect* Owner = GetOwner();
	Owner->CachedAssetTags.AppendTags(DamageTags);
}

bool URsDamageEffectComponent::CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	float StatValue = URsAbilitySystemLibrary::GetNumericAttributeByTag(ActiveGEContainer.Owner, RsGameplayTags::STAT_INV);
	return StatValue <= InvinciblePierce;
}
