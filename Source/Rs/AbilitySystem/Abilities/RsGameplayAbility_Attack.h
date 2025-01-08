// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility.h"
#include "RsGameplayAbility_Attack.generated.h"

class UTargetingPreset;
/**
 * 
 */
UCLASS()
class RS_API URsGameplayAbility_Attack : public URsGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	UAnimMontage* MontageToPlay;
	
	// Tag that define hit detect or fire projectile event.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage", meta = (Categories = "Event"))
	FGameplayTag AttackEventTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Focus")
	UTargetingPreset* FocusTargetingPreset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Focus")
	float RotatingSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Focus")
	float RotatingMaxDuration = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage")
	float DamageCoefficient = 1.f;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void HandleMontageCompleted();

	UFUNCTION()
	void HandleMontageCancelled();
};
