// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
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

	virtual void ApplyDamageDefinition(const AActor* Source, const AActor* Target, int32 InvinciblePierce) const;

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
	virtual void ApplyDamageDefinition(const AActor* Source, const AActor* Target, int32 InvinciblePierce) const override;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1000"))
	float Period = 0.5f;
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
};

/**
 * Use this to select a specific damage effect class.
 */
UCLASS(DisplayName = "Custom Damage")
class RS_API URsDamageDefinition_Custom : public URsDamageDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRsEffectCoefficient CustomEffect;
};