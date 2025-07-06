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
	FGameplayTagContainer AdditionalDamageTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Ability.HitReaction"))
	FGameplayTag HitReaction;
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Amount")
	float DamageMultiplierPerDotStacks;

public:
	virtual void ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};

/**
 * Buff effect that needs duration.
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
	PlusMinus,
	Override
};

UCLASS(DisplayName = "Modify Cooldown")
class RS_API URsEffectDefinition_ModifyCooldown : public URsEffectDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cooldown"))
	FGameplayTag CooldownTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECooldownModifingType ModifingType = ECooldownModifingType::PlusMinus;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount = 0.f;

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
	FRsEffectCoefficient CustomEffect;

public:
	virtual void ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};
