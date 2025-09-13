// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "RsBuffEffect.h"
#include "RsEffectCoefficient.h"
#include "RsEffectDefinition.generated.h"

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
	virtual void ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) PURE_VIRTUAL(URsEffectDefinition::ApplyEffect, );
};

UCLASS(Abstract)
class RS_API URsEffectDefinition_DamageBase : public URsEffectDefinition
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;
	virtual void ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override {};
	
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient,Manual.Magnitude", ForceInlineRow))
	TMap<FGameplayTag, float> HealthDamageCoefficients;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient,Manual.Magnitude", ForceInlineRow))
	TMap<FGameplayTag, float> StaggerDamageCoefficients;

public:
	virtual void ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient,Manual.Magnitude", ForceInlineRow))
	TMap<FGameplayTag, float> HealthDamageCoefficients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient,Manual.Magnitude", ForceInlineRow))
	TMap<FGameplayTag, float> StaggerDamageCoefficients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Period = 0.5f;

public:
	virtual void ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
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
	virtual void ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};

/**
 * Buff effect that has duration.
 */
UCLASS(DisplayName = "Buff")
class RS_API URsEffectDefinition_Buff : public URsEffectDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<URsBuffEffect> BuffClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient,Manual.Magnitude", ForceInlineRow))
	TMap<FGameplayTag, float> Coefficients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0.f;

public:
	virtual void ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};

UENUM()
enum class ECooldownModifingType
{
	Add,
	Set
};

/**
 * Changes the cooldown of an ability.
 */
UCLASS(DisplayName = "Change Cooldown")
class RS_API URsEffectDefinition_ChangeCooldown : public URsEffectDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cooldown"))
	FGameplayTag CooldownTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECooldownModifingType ModifingType = ECooldownModifingType::Add;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount = 0.f;

public:
	virtual void ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};

/**
 * Gain energy for using ultimate skill.
 */
UCLASS(DisplayName = "Gain Energy")
class RS_API URsEffectDefinition_GainEnergy : public URsEffectDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount = 10.f;

public:
	virtual void ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};

/**
 * Gain current health based on target's damage.
 * HealthGain = (HealthDamage * DamageMultiplier)
 */
UCLASS(DisplayName = "Lifesteal")
class RS_API URsEffectDefinition_Lifesteal : public URsEffectDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier = 0.1f;

public:
	virtual void ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};

UCLASS(DisplayName = "Hit Stop")
class RS_API URsEffectDefinition_HitStop : public URsEffectDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SourceHitStopTime = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetHitStopTime = 0.2f;

public:
	virtual void ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
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
	TSubclassOf<UGameplayEffect> CustomEffect;

public:
	virtual void ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};
