// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility.h"
#include "RsGameplayAbility_Melee.generated.h"

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS", meta = (Categories = "Event"))
	FGameplayTag HitDetectEventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS")
	UTargetingPreset* HitDetectTargetingPreset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS")
	TSubclassOf<UGameplayEffect> DamageEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void HandleMontageEnd();

	UFUNCTION()
	void HandleHitDetectEvent(FGameplayEventData EventData);
};
