// Copyright 2024 Team BH.


#include "RsAbilityTask_WaitDamageEffectBlockedImmunity.h"

URsAbilityTask_WaitDamageEffectBlockedImmunity* URsAbilityTask_WaitDamageEffectBlockedImmunity::WaitDamageEffectBlockedByImmunity(UGameplayAbility* OwningAbility, FGameplayTagContainer DamageTags, bool TriggerOnce)
{
	URsAbilityTask_WaitDamageEffectBlockedImmunity* MyObj = NewAbilityTask<URsAbilityTask_WaitDamageEffectBlockedImmunity>(OwningAbility);
	MyObj->TriggerOnce = TriggerOnce;
	MyObj->DamageTags = DamageTags;
	return MyObj;
}

void URsAbilityTask_WaitDamageEffectBlockedImmunity::ImmunityCallback(const FGameplayEffectSpec& BlockedSpec, const FActiveGameplayEffect* ImmunityGE)
{
	bool PassedComparison = false;

	FGameplayTagContainer GEAssetTags;
	BlockedSpec.GetAllAssetTags(GEAssetTags);
	if (!GEAssetTags.HasAll(DamageTags))
	{
		return;
	}
	
	// We have to copy the spec, since the blocked spec is not ours
	FGameplayEffectSpecHandle SpecHandle(new FGameplayEffectSpec(BlockedSpec));

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		Blocked.Broadcast(SpecHandle, ImmunityGE->Handle);
	}
	
	if (TriggerOnce)
	{
		EndTask();
	}
}
