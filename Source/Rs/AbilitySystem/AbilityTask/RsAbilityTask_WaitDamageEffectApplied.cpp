// Copyright 2024 Team BH.


#include "RsAbilityTask_WaitDamageEffectApplied.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"

URsAbilityTask_WaitDamageEffectApplied* URsAbilityTask_WaitDamageEffectApplied::WaitDamageEffect(UGameplayAbility* OwningAbility, FGameplayTagContainer DamageTags, bool TriggerOnce)
{
	URsAbilityTask_WaitDamageEffectApplied* MyObj = NewAbilityTask<URsAbilityTask_WaitDamageEffectApplied>(OwningAbility);
	MyObj->DamageTags = DamageTags;
	MyObj->TriggerOnce = TriggerOnce;
	return MyObj;
}

void URsAbilityTask_WaitDamageEffectApplied::BroadcastDelegate(AActor* Avatar, FGameplayEffectSpecHandle SpecHandle, FActiveGameplayEffectHandle ActiveHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnApplied.Broadcast(Avatar, SpecHandle, ActiveHandle);
	}
}

void URsAbilityTask_WaitDamageEffectApplied::RegisterDelegate()
{
	OnApplyGameplayEffectCallbackDelegateHandle = GetASC()->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::OnApplyDamageEffectCallback);
	if (ListenForPeriodicEffects)
	{
		OnPeriodicGameplayEffectExecuteCallbackDelegateHandle = GetASC()->OnPeriodicGameplayEffectExecuteDelegateOnSelf.AddUObject(this, &ThisClass::OnApplyDamageEffectCallback);
	}
}

void URsAbilityTask_WaitDamageEffectApplied::RemoveDelegate()
{
	GetASC()->OnGameplayEffectAppliedDelegateToSelf.Remove(OnApplyGameplayEffectCallbackDelegateHandle);
	if (OnPeriodicGameplayEffectExecuteCallbackDelegateHandle.IsValid())
	{
		GetASC()->OnPeriodicGameplayEffectExecuteDelegateOnSelf.Remove(OnPeriodicGameplayEffectExecuteCallbackDelegateHandle);
	}
}

void URsAbilityTask_WaitDamageEffectApplied::OnApplyDamageEffectCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	bool PassedComparison = false;

	AActor* AvatarActor = Target ? Target->GetAvatarActor_Direct() : nullptr;

	if (Locked)
	{
		ABILITY_LOG(Error, TEXT("WaitGameplayEffectApplied recursion detected. Ability: %s. Applied Spec: %s. This could cause an infinite loop! Ignoring"), *GetNameSafe(Ability), *SpecApplied.ToSimpleString());
		return;
	}

	FGameplayTagContainer GEAssetTags;
	SpecApplied.GetAllAssetTags(GEAssetTags);
	if (!GEAssetTags.HasAll(DamageTags))
	{
		return;
	}
	
	FGameplayEffectSpecHandle	SpecHandle(new FGameplayEffectSpec(SpecApplied));

	{
		TGuardValue<bool> GuardValue(Locked, true);	
		BroadcastDelegate(AvatarActor, SpecHandle, ActiveHandle);
	}

	if (TriggerOnce)
	{
		EndTask();
	}
}
