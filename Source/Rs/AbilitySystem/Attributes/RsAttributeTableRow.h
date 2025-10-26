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
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// float Level = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Barrier = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxStagger = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentStagger = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StaggerDecay = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMana = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentMana = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxEnergy = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentEnergy = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Attack = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Impact = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Defense = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CriticalRate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CriticalDamage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Invincible = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SuperArmor = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveSpeed  = 1.f;
	
	float GetBaseValue(const FGameplayAttribute& Attribute) const;
};
