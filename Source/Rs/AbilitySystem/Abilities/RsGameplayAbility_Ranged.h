// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility.h"
#include "RsGameplayAbility_Ranged.generated.h"

class ARsProjectile;
class UTargetingPreset;

/**
 * 
 */
UCLASS()
class RS_API URsGameplayAbility_Ranged : public URsGameplayAbility
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

	// If Projectile is none, deal instant damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage")
	TSubclassOf<ARsProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage", meta = (Categories = "Event"))
	FGameplayTag FireProjectileEventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void HandleMontageCompleted();

	UFUNCTION()
	void HandleMontageCancelled();

	UFUNCTION()
	void HandleFireProjectile(FGameplayEventData EventData);

	UFUNCTION()
	void HandleInstantDamage();

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> CachedVictim = nullptr;
};
