// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility_Attack.h"
#include "RsGameplayAbility_Melee.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UTargetingPreset;
/**
 * 
 */
UCLASS()
class RS_API URsGameplayAbility_Melee : public URsGameplayAbility_Attack
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage")
	UTargetingPreset* DamageTargetingPreset;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void HandleHitDetect(FGameplayEventData EventData);

private:
	UPROPERTY(Transient)
	UAbilityTask_WaitGameplayEvent* HitDetectTask = nullptr;
};
