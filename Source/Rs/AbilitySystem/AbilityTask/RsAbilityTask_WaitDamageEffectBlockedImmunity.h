// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectBlockedImmunity.h"
#include "RsAbilityTask_WaitDamageEffectBlockedImmunity.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWaitDamageEffectBlocked, FGameplayEffectSpecHandle, BlockedSpec, FActiveGameplayEffectHandle, ImmunityGameplayEffectHandle);


/**
 * 
 */
UCLASS()
class RS_API URsAbilityTask_WaitDamageEffectBlockedImmunity : public UAbilityTask_WaitGameplayEffectBlockedImmunity
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTagContainer DamageTags;

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URsAbilityTask_WaitDamageEffectBlockedImmunity* WaitDamageEffectBlockedByImmunity(UGameplayAbility* OwningAbility, FGameplayTagContainer InDamageTags, bool bTriggerOnce = false);

protected:
	virtual void ImmunityCallback(const FGameplayEffectSpec& BlockedSpec, const FActiveGameplayEffect* ImmunityGE) override;
	virtual void OnDestroy(bool AbilityEnded) override;
};
