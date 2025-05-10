// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "RsEffectCoefficient.h"
#include "RsDamageDefinition.generated.h"

struct FGameplayEffectSpec;
struct FGameplayEffectSpecHandle;
class URsDeveloperSetting;
class UAbilitySystemComponent;
/**
 * Datas to make damage GE specs.
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract, CollapseCategories)
class RS_API URsDamageDefinition : public UObject
{
	GENERATED_BODY()

public:
	URsDamageDefinition();

	void SetInvinciblePierce(int32 InvinciblePierce);

	FGameplayEffectContextHandle MakeDamageEffectContext(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC);
	virtual void ApplyDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC);

	UPROPERTY()
	int32 InvinciblePierce = 0;

protected:
	UPROPERTY()
	const URsDeveloperSetting* DeveloperSetting;
};

/**
 * Deal damage instantly. The most general type of damage.
 * (Ex: Melee hit)
 */
UCLASS(DisplayName = "Instant Damage")
class RS_API URsDamageDefinition_Instant : public URsDamageDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient,Manual.Magnitude", ForceInlineRow))
	TMap<FGameplayTag, float> HealthDamageCoefficients;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient,Manual.Magnitude", ForceInlineRow))
	TMap<FGameplayTag, float> StaggerDamageCoefficients;

public:
	virtual void ApplyDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};

/**
 * Damage Over Time. It has duration and period.
 * (Ex: Poison) 
 */
UCLASS(DisplayName = "DoT Damage")
class RS_API URsDamageDefinition_Dot : public URsDamageDefinition
{
	GENERATED_BODY()

public:
	// Total sum of the health damage until duration end.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient,Manual.Magnitude", ForceInlineRow))
	TMap<FGameplayTag, float> HealthDamageCoefficients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient,Manual.Magnitude", ForceInlineRow))
	TMap<FGameplayTag, float> StaggerDamageCoefficients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1000"))
	float Period = 0.5f;

public:
	virtual void ApplyDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};

/**
 * Instantly deals the remaining damage from all active DoT stacks.
 * (Ex: Bloodburst) 
 */
UCLASS(DisplayName = "DoT Burst Damage")
class RS_API URsDamageDefinition_DotBurst : public URsDamageDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplierPerDotStacks;

public:
	virtual void ApplyDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};

/**
 * Use this to select a specific effect class.
 */
UCLASS(DisplayName = "Custom")
class RS_API URsDamageDefinition_Custom : public URsDamageDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRsEffectCoefficient CustomEffect;

public:
	virtual void ApplyDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) override;
};