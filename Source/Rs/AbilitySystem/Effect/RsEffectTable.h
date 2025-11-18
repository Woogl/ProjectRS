// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsEffectTable.generated.h"

class UGameplayEffect;
/**
 * 
 */
USTRUCT()
struct FRsEffectTableRowBase : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
};

USTRUCT(BlueprintType)
struct FRsEffectTableRow : public FRsEffectTableRowBase
{
	GENERATED_BODY()
	
	/** ex1: 170
	 * ex2: (Coefficient.ATK.Source * 1.5) + 30 */
	UPROPERTY(EditDefaultsOnly)
	FString MagnitudeExpression;

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
	FText Title;
	
	UPROPERTY(EditDefaultsOnly)
	FText Description;
};
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRsDamageTableRow : public FRsEffectTableRowBase
{
	GENERATED_BODY()
	
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
