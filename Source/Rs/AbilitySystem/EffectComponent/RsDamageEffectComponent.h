// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"
#include "RsDamageEffectComponent.generated.h"

struct FRsDamageTableRow;
/**
 * It contains datas that make damage.
 * AssetTags + CanApply + ModifierData + HitReaction + HitStop
 */
UCLASS()
class RS_API URsDamageEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	URsDamageEffectComponent();
	
	/** Modifiers data */
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, float> HealthDamageCoefficients;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, float> StaggerDamageCoefficients;

	/** Can apply */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", ClampMax = "3"))
	bool InvinciblePierce = false;
	
	/** Hit reaction */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", ClampMax = "3"))
	bool SuperArmorPierce = false;
	
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Ability.HitReaction"))
	FGameplayTag HitReaction;
	
	/** Hit stop */
	UPROPERTY(EditDefaultsOnly)
	float SourceHitStopTime = 0.f;
	
	UPROPERTY(EditDefaultsOnly)
	float TargetHitStopTime = 0.f;
	
	/** Advantage to source */
	UPROPERTY(EditDefaultsOnly)
	float ManaGain = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float UltimateGain = 0.f;
	
	/** GE asset tags */
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Effect.Damage"))
	FGameplayTagContainer DamageTags;

public:
	// Handle DamageTags
	virtual void OnGameplayEffectChanged() override;

	// Handle HitReaction, HitStops, ManaGain, UltimateGain, AdditionalEffects
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
};
