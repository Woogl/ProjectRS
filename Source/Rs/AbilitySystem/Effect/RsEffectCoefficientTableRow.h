// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsEffectCoefficientTableRow.generated.h"

class URsUnitEffect;
class URsGameplayEffect;

USTRUCT(BlueprintType)
struct RS_API FRsEffectCoefficientTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URsUnitEffect> EffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Coefficient"))
	FGameplayTag Tag1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Coefficient"))
	FGameplayTag Tag2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Coefficient"))
	FGameplayTag Tag3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value3;
};
