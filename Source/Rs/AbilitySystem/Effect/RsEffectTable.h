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
USTRUCT()
struct FRsEffectTableRowBase : public FTableRowBase
{
	GENERATED_BODY()

	// Referencing GE asset. Grant to target.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Effect"))
	FGameplayTag EffectTag;
};

// TODO: Implement FRsEffectTableRow
USTRUCT(BlueprintType)
struct FRsEffectTableRow : public FRsEffectTableRowBase
{
	GENERATED_BODY()
	
	/** Magnitude */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Magnitude;

	/** Time */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Duration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Period = 0.f;

	/** Polymorphic parameters (optional) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Parameter1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Parameter2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Parameter3;

	/** Additional Effect */
	UPROPERTY(EditDefaultsOnly)
	FString AdditionalSourceEffect;

	UPROPERTY(EditDefaultsOnly)
	FString AdditionalTargetEffect;
};
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRsDamageTableRow : public FRsEffectTableRowBase
{
	GENERATED_BODY()

	FRsDamageTableRow();

	/** Magnitude */
	UPROPERTY(EditAnywhere)
	FString HealthDamageExpression;

	UPROPERTY(EditAnywhere)
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
	FString AdditionalSourceEffect;

	UPROPERTY(EditDefaultsOnly)
	FString AdditionalTargetEffect;
};
