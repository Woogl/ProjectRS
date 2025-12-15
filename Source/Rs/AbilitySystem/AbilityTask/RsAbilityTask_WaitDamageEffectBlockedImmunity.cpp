// Copyright 2024 Team BH.


#include "RsAbilityTask_WaitDamageEffectBlockedImmunity.h"

#include "Components/CapsuleComponent.h"
#include "Rs/Character/RsPlayerCharacter.h"

void URsAbilityTask_WaitDamageEffectBlockedImmunity::Activate()
{
	Super::Activate();

	if (bEnablePerfectDodgeCapsule == true)
	{
		if (ARsPlayerCharacter* RsPlayerCharacter = Cast<ARsPlayerCharacter>(GetAvatarActor()))
		{
			//RsPlayerCharacter->GetPerfectDodgeCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}
}

URsAbilityTask_WaitDamageEffectBlockedImmunity* URsAbilityTask_WaitDamageEffectBlockedImmunity::WaitDamageEffectBlockedByImmunity(UGameplayAbility* OwningAbility, FGameplayTagContainer DamageTags, bool bEnablePerfectDodgeCapsule, bool TriggerOnce)
{
	URsAbilityTask_WaitDamageEffectBlockedImmunity* MyObj = NewAbilityTask<URsAbilityTask_WaitDamageEffectBlockedImmunity>(OwningAbility);
	MyObj->TriggerOnce = TriggerOnce;
	MyObj->DamageTags = DamageTags;
	MyObj->bEnablePerfectDodgeCapsule = bEnablePerfectDodgeCapsule;
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

void URsAbilityTask_WaitDamageEffectBlockedImmunity::OnDestroy(bool AbilityEnded)
{
	if (bEnablePerfectDodgeCapsule == true)
	{
		if (ARsPlayerCharacter* RsPlayerCharacter = Cast<ARsPlayerCharacter>(GetAvatarActor()))
		{
			//RsPlayerCharacter->GetPerfectDodgeCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	
	Super::OnDestroy(AbilityEnded);
}
