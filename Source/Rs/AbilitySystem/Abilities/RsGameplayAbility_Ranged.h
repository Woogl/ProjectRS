// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility_Attack.h"
#include "RsGameplayAbility_Ranged.generated.h"

class ARsProjectile;

/**
 * 
 */
UCLASS()
class RS_API URsGameplayAbility_Ranged : public URsGameplayAbility_Attack
{
	GENERATED_BODY()

protected:
	// Tag that define fire projectile event
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage", meta = (Categories = "AnimNotify"))
	FGameplayTag FireEventTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage")
	TSubclassOf<ARsProjectile> ProjectileClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void HandleFireProjectile(FGameplayEventData EventData);

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> CachedVictim = nullptr;
};
