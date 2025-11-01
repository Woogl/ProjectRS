// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"
#include "RsDamageEffectComponent.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FRsDamageEffectTableRow : public FTableRowBase
{
	GENERATED_BODY()

	/** Modifiers */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString HealthDamageExpression;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString StaggerDamageExpression;

	/** Can apply */
	UPROPERTY(EditDefaultsOnly)
	int32 InvinciblePierce = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 SuperArmorPierce = 0;

	/** Hit reaction */
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Ability.HitReaction"))
	FGameplayTag HitReaction;

	/** Hit stop */
	UPROPERTY(EditDefaultsOnly)
	float SourceHitStopTime = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float TargetHitStopTime = 0.f;

	/** GE asset tags */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories = "Effect.Damage"))
	FGameplayTagContainer DamageTags;
};

/**
 * It contains datas that make damage.
 * AssetTags + CanApply + ModifierData + CrowdControl + HitStop
 */
UCLASS()
class RS_API URsDamageEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	/** Modifiers */
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, float> HealthDamageCoefficients;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, float> StaggerDamageCoefficients;
#endif // WITH_EDITORONLY_DATA

	/** Can apply */
	UPROPERTY(EditDefaultsOnly)
	int32 InvinciblePierce = 0;
	
	/** Hit reaction */
	UPROPERTY(EditDefaultsOnly)
	int32 SuperArmorPierce = 0;
	
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Ability.HitReaction"))
	FGameplayTag HitReaction;
	
	/** Hit stop */
	UPROPERTY(EditDefaultsOnly)
	float SourceHitStopTime = 0.f;
	
	UPROPERTY(EditDefaultsOnly)
	float TargetHitStopTime = 0.f;

	/** GE asset tags */
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Effect.Damage"))
	FGameplayTagContainer DamageTags;

	/** From data table */
	UPROPERTY(EditDefaultsOnly, meta = (RowType = "RsDamageEffectTableRow"))
	FDataTableRowHandle DataTableRow;

public:
	// Handle DamageTags
	virtual void OnGameplayEffectChanged() override;
	
	// Handle InvinciblePierce
	virtual bool CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const override;
};
