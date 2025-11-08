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
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Effect"))
	FGameplayTag EffectTag;
};

USTRUCT(BlueprintType)
struct FRsEffectTableRow : public FRsEffectTableRowBase
{
	GENERATED_BODY()
	
	/** ex1: 170
	 * ex2: (Coefficient.ATK.Source * 1.5) + 30 */
	UPROPERTY(EditDefaultsOnly)
	FString Magnitude;

	/** Time */
	UPROPERTY(EditDefaultsOnly)
	float Duration = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float Period = 0.f;

	/** Polymorphic parameters (optional) */
	UPROPERTY(EditDefaultsOnly)
	FString Parameter1;

	UPROPERTY(EditDefaultsOnly)
	FString Parameter2;

	UPROPERTY(EditDefaultsOnly)
	FString Parameter3;

	/** UI Data */
	UPROPERTY(EditDefaultsOnly)
	FText EffectName;
	
	UPROPERTY(EditDefaultsOnly)
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowedClasses="MaterialInterface,Texture2D"))
	TSoftObjectPtr<UObject> Icon = nullptr;

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
