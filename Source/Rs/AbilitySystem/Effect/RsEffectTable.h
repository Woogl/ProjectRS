// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "RsEffectTable.generated.h"

class UGameplayEffect;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRsEffectTableRowBase : public FTableRowBase
{
	GENERATED_BODY()

	// Referencing GE asset. Grant to target.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Effect"))
	FGameplayTag EffectTag;
};

// TODO: Implement FRsEffectTableRow
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRsDamageTableRow : public FRsEffectTableRowBase
{
	GENERATED_BODY()

	FRsDamageTableRow();

	/** Modifiers */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString HealthDamageExpression;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString StaggerDamageExpression;

	/** Can apply */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", ClampMax = "3"))
	int32 InvinciblePierce = 0;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", ClampMax = "3"))
	int32 SuperArmorPierce = 0;

	/** Hit reaction */
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

	/** Additional Effect */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> AdditionalSourceEffect;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> AdditionalTargetEffect;
};
