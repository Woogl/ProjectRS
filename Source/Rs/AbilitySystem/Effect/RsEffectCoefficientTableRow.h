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
	TSubclassOf<URsUnitEffect> UnitEffect1;

	// ex. (ATK.source * 1.5) + 500
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CoefficientExpression1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URsUnitEffect> UnitEffect2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CoefficientExpression2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URsUnitEffect> UnitEffect3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CoefficientExpression3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URsUnitEffect> UnitEffect4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CoefficientExpression4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URsUnitEffect> UnitEffect5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CoefficientExpression5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URsUnitEffect> UnitEffect6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CoefficientExpression6;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
};
