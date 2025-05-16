// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility.h"
#include "RsGameplayAbility_Attack.generated.h"

class URsDamageDefinition;

USTRUCT(BlueprintType)
struct FRsDamageContext
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories = "Animnotify.HitScan"))
	FGameplayTag DamageEventTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TObjectPtr<URsDamageDefinition>> DamageDefinitions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "3"))
	int32 InvinciblePierce;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "3"))
	int32 SuperArmorPierce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Ability.HitReaction"))
	FGameplayTag HitReaction;

	bool operator==(const FRsDamageContext& Other) const { return this->DamageEventTag == Other.DamageEventTag; }
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage", meta = (Categories = "AnimNotify", ForceInlineRow, TitleProperty="DamageEventTag"))
	TArray<FRsDamageContext> DamageEvents;
	
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
