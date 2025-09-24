// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "RsInvincibleGameplayEffectComponent.generated.h"

struct FActiveGameplayEffectHandle;
/**
 * 
 */
UCLASS(Deprecated)
class RS_API UDEPRECATED_RsInvincibleGameplayEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	friend class UGameplayEffect; // Needed for upgrade path
	UDEPRECATED_RsInvincibleGameplayEffectComponent();

	/** We need to register our callback to check for Immunity */
	virtual bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const override;

#if WITH_EDITOR
	/**
	 * Warn about instant Gameplay Effects as they do not persist over time (and therefore cannot grant immunity)
	 */
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR

protected:
	/** We register with the AbilitySystemComponent to try and block any GESpecs we think we should be immune to */
	bool AllowGameplayEffectApplication(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpecToConsider, FActiveGameplayEffectHandle ImmunityActiveGE) const;
};
