// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility.h"
#include "RsGameplayAbility_Melee.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UTargetingPreset;
/**
 * 
 */
UCLASS()
class RS_API URsGameplayAbility_Melee : public URsGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS")
	UAnimMontage* MontageToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Focus")
	UTargetingPreset* FocusTargetingPreset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Focus")
	float RotatingSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Focus")
	float RotatingMaxDuration = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage")
	UTargetingPreset* DamageTargetingPreset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage", meta = (Categories = "Event"))
	FGameplayTag HitDetectEventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void HandleMontageCompleted();

	UFUNCTION()
	void HandleMontageCancelled();
	
	UFUNCTION()
	void HandleHitDetectEvent(FGameplayEventData EventData);

private:
	UPROPERTY(Transient)
	UAbilityTask_WaitGameplayEvent* HitDetectTask = nullptr;
};
