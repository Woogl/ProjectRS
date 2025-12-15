// Copyright 2024 Team BH.


#include "RsAbilityTask_WaitDamageEffectApplied.h"

#include "AbilitySystemComponent.h"

void URsAbilityTask_WaitDamageEffectApplied::Activate()
{
	Super::Activate();
}

URsAbilityTask_WaitDamageEffectApplied* URsAbilityTask_WaitDamageEffectApplied::WaitDamageEffect(UGameplayAbility* OwningAbility, FGameplayTagContainer DamageTags, bool bEnablePerfectDodgeCapsule, bool TriggerOnce)
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

void URsAbilityTask_WaitDamageEffectApplied::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}

void URsAbilityTask_WaitDamageEffectApplied::OnApplyDamageEffectCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer GEAssetTags;
	SpecApplied.GetAllAssetTags(GEAssetTags);
	if (!GEAssetTags.HasAll(DamageTags))
	{
		return;
	}
	
	FGameplayEffectSpecHandle	SpecHandle(new FGameplayEffectSpec(SpecApplied));

	if (TriggerOnce)
	{
		EndTask();
	}
}
