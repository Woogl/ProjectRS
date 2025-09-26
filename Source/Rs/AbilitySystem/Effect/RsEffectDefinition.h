// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "RsEffectCoefficient.h"
#include "RsEffectDefinition.generated.h"

class URsGameplayEffect;
struct FGameplayEffectSpec;
struct FGameplayEffectSpecHandle;
class URsDeveloperSetting;
class UAbilitySystemComponent;

/**
 * Datas to make GE specs.
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract, CollapseCategories)
class RS_API URsEffectDefinition : public UObject
{
	GENERATED_BODY()

public:
	virtual FActiveGameplayEffectHandle ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) PURE_VIRTUAL(URsEffectDefinition::ApplyEffect, return FActiveGameplayEffectHandle(););
};

UCLASS(Abstract)
class RS_API URsEffectDefinition_DamageBase : public URsEffectDefinition
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;
	virtual FActiveGameplayEffectHandle ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "3"))
	int32 InvinciblePierce;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "3"))
	int32 SuperArmorPierce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Effect.Damage"))
	FGameplayTagContainer DamageTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Ability.HitReaction"))
	FGameplayTag HitReaction = FGameplayTag::RequestGameplayTag(TEXT("Ability.HitReaction.Normal"));
	
	FGameplayEffectContextHandle MakeDamageEffectContext(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) const;
	void ApplyInstantDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FRsEffectCoefficient& RsCoeff);
	void ApplyDotDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FRsEffectCoefficient& RsCoeff, float Duration, float Period);
	void ApplyHitReaction(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC);
	
protected:
	UPROPERTY()
	const URsDeveloperSetting* DeveloperSetting;
};

/**
 * Deal damage instantly. The most general type of damage.
 * (Ex: Melee hit)
 */
UCLASS(DisplayName = "Instant Damage")
class RS_API URsEffectDefinition_InstantDamage : public URsEffectDefinition_DamageBase
{
	GENERATED_BODY()

public:
	URsEffectDefinition_InstantDamage();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, float> HealthDamageCoefficients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, float> StaggerDamageCoefficients;

public:
	virtual FActiveGameplayEffectHandle ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};

/**
 * Damage Over Time. It has duration and period.
 * (Ex: Poison) 
 */
UCLASS(DisplayName = "DoT Damage")
class RS_API URsEffectDefinition_DotDamage : public URsEffectDefinition_DamageBase
{
	GENERATED_BODY()

public:
	// Total sum of the health damage until duration end.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, float> HealthDamageCoefficients;

	// Total sum of the stagger damage until duration end.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, float> StaggerDamageCoefficients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Period = 0.5f;

public:
	virtual FActiveGameplayEffectHandle ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};

/**
 * Instantly deals the remaining damage from all active DoT stacks.
 * (Ex: Disorder from Zenless Zone Zero) 
 */
UCLASS(DisplayName = "DoT Burst Damage")
class RS_API URsEffectDefinition_DotBurstDamage : public URsEffectDefinition_DamageBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplierPerDotStacks = 0.05f;

public:
	virtual FActiveGameplayEffectHandle ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};

/**
 * Apply RsGameplayEffect with coefficients.
 */
UCLASS(DisplayName = "Effect Coefficient")
class RS_API URsEffectDefinition_EffectCoefficient : public URsEffectDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<URsGameplayEffect> Effect;

public:
	virtual FActiveGameplayEffectHandle ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};

/**
 * Use this to select a specific effect class.
 */
UCLASS(DisplayName = "Custom")
class RS_API URsEffectDefinition_Custom : public URsEffectDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, float> SetByCallerDatas;

public:
	virtual FActiveGameplayEffectHandle ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};