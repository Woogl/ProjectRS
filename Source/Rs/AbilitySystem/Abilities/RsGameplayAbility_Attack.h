// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficient.h"
#include "RsGameplayAbility_Attack.generated.h"

USTRUCT(BlueprintType)
struct FRsDamageEventContext
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Damage Event", Categories = "Animnotify.HitScan"))
	FGameplayTag DamageEventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRsEffectCoefficient> EffectCoefficients;

	bool operator==(const FRsDamageEventContext& Other) const { return this->DamageEventTag == Other.DamageEventTag; }
	bool operator==(const FGameplayTag& Other) const { return this->DamageEventTag == Other; }
};

/**
 * 
 */
UCLASS()
class RS_API URsGameplayAbility_Attack : public URsGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage", meta = (Categories = "AnimNotify", ForceInlineRow))
	TArray<FRsDamageEventContext> DamageEvents;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void HandleMontageCompleted();

	UFUNCTION()
	void HandleMontageCancelled();

public:
	void OnAttackHitTarget(const AActor* Target, const FGameplayTag& DamageEvent);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "RS|Damage", DisplayName = "OnAttackHitTarget")
	void K2_OnAttackHitTarget(const AActor* Target, const FGameplayTag& DamageEvent);
};
