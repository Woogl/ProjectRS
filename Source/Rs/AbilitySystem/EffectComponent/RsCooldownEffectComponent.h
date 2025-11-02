// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"
#include "RsCooldownEffectComponent.generated.h"

UENUM()
enum class ECooldownModifingType
{
	Add,
	Set
};

/**
 * 
 */
UCLASS()
class RS_API URsCooldownEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Cooldown"))
	FGameplayTag CooldownTag;
	
	UPROPERTY(EditDefaultsOnly)
	ECooldownModifingType ModifingType = ECooldownModifingType::Set;
	
	UPROPERTY(EditDefaultsOnly)
	float Amount = 0.f;
	
public:
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
};
