// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayEffectTypes.h"
#include "RsCanApplyEffectComponent.generated.h"

UENUM(BlueprintType)
enum class ERsComparisionOperator : uint8
{
	None,
	Greater,
	Equal,
	Less,
};

/**
 * 
 */
UCLASS()
class RS_API URsCanApplyEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Stat"))
	FGameplayTag Stat;

	UPROPERTY(EditDefaultsOnly)
	ERsComparisionOperator Comparision = ERsComparisionOperator::None;

	UPROPERTY(EditDefaultsOnly)
	float Value = 0.f;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagRequirements TagRequirements;

	UPROPERTY(EditDefaultsOnly)
	bool bNotifyImmunityBlock = true;

public:
	URsCanApplyEffectComponent();
	
	/** Determine if we can apply this GameplayEffect or not */
	virtual bool CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const override;
	
	bool ShouldImmunityBlock(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const;
};
