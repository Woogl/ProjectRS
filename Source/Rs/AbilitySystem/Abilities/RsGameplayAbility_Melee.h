// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility_Attack.h"
#include "RsGameplayAbility_Melee.generated.h"

class UAbilityTask_WaitGameplayEvent;

/**
 * 
 */
UCLASS()
class RS_API URsGameplayAbility_Melee : public URsGameplayAbility_Attack
{
	GENERATED_BODY()

protected:
	// Tag that define hit detect event
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage", meta = (Categories = "Event"))
	FGameplayTag HitDetectEventTag;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void HandleHitDetect(FGameplayEventData EventData);
};
