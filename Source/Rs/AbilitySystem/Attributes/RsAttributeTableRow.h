// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeTableRow.generated.h"

/**
 * 
 */
struct FGameplayAttribute;
/**
 * 
 */
USTRUCT(BlueprintType)
struct RS_API FRsAttributeTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// UPROPERTY(EditAnywhere)
	// float Level = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 0.f;

	UPROPERTY(EditAnywhere)
	float CurrentHealth = 0.f;

	UPROPERTY(EditAnywhere)
	float Barrier = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxStagger = 0.f;

	UPROPERTY(EditAnywhere)
	float CurrentStagger = 0.f;

	UPROPERTY(EditAnywhere)
	float StaggerDecay = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxMana = 0.f;

	UPROPERTY(EditAnywhere)
	float CurrentMana = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxUltimate = 0.f;

	UPROPERTY(EditAnywhere)
	float CurrentUltimate = 0.f;

	UPROPERTY(EditAnywhere)
	float Attack = 0.f;

	UPROPERTY(EditAnywhere)
	float Impact = 0.f;

	UPROPERTY(EditAnywhere)
	float Defense = 0.f;

	UPROPERTY(EditAnywhere)
	float CriticalRate = 0.f;

	UPROPERTY(EditAnywhere)
	float CriticalDamage = 0.f;
	
	float GetBaseValue(const FGameplayAttribute& Attribute) const;
};
