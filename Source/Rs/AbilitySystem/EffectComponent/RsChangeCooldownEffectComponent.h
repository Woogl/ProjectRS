// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"
#include "Rs/AbilitySystem/Effect/RsEffectDefinition.h"
#include "RsChangeCooldownEffectComponent.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsChangeCooldownEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cooldown"))
	FGameplayTag CooldownTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECooldownModifingType ModifingType = ECooldownModifingType::Add;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount = 0.f;
};
