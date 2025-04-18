// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility.h"
#include "RsGameplayAbility_Attack.generated.h"

USTRUCT(BlueprintType)
struct FRsDamageEventContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RS|Damage", meta = (Categories = "Effect.Damage"))
	FGameplayTagContainer DamageEffectTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage")
	float HealthDamageCoefficient = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage")
	float StaggerDamageCoefficient = 1.f;
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
	TMap<FGameplayTag, FRsDamageEventContext> DamageEvents;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void HandleMontageCompleted();

	UFUNCTION()
	void HandleMontageCancelled();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bHasHitTarget = false;
	
	void OnAttackHitTarget(const AActor* Target, const FGameplayTag& DamageEvent);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "RS|Damage", DisplayName = "OnAttackHitTarget")
	void K2_OnAttackHitTarget(const AActor* Target, const FGameplayTag& DamageEvent);
};
