// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectApplied.h"
#include "RsAbilityTask_WaitDamageEffectApplied.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FWaitDamageEffectDelegate, AActor*, Source, FGameplayEffectSpecHandle, SpecHandle,  FActiveGameplayEffectHandle, ActiveHandle );

/**
 * 
 */
UCLASS()
class RS_API URsAbilityTask_WaitDamageEffectApplied : public UAbilityTask_WaitGameplayEffectApplied
{
	GENERATED_BODY()

	FGameplayTagContainer DamageTags;
	virtual void Activate() override;
	
public:
	UPROPERTY(BlueprintAssignable)
	FWaitDamageEffectDelegate OnApplied;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URsAbilityTask_WaitDamageEffectApplied* WaitDamageEffect(UGameplayAbility* OwningAbility, FGameplayTagContainer InDamageTags, bool TriggerOnce = false);

protected:
	virtual void BroadcastDelegate(AActor* Avatar, FGameplayEffectSpecHandle SpecHandle, FActiveGameplayEffectHandle ActiveHandle) override;
	virtual void RegisterDelegate() override;
	virtual void RemoveDelegate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	UFUNCTION()
	void OnApplyDamageEffectCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);

private:
	FDelegateHandle OnApplyGameplayEffectCallbackDelegateHandle;
	FDelegateHandle OnPeriodicGameplayEffectExecuteCallbackDelegateHandle;
};
