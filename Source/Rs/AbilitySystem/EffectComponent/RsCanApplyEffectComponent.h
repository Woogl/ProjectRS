// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayEffectTypes.h"
#include "RsCanApplyEffectComponent.generated.h"

UENUM(BlueprintType)
enum class ERsStatComparision : uint8
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

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Stat"))
	FGameplayTag Stat;

	UPROPERTY(EditDefaultsOnly)
	ERsStatComparision Comparision = ERsStatComparision::None;

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
