// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"
#include "RsModifierDataEffectComponent.generated.h"

// Set modifiers from asset
USTRUCT(BlueprintType)
struct FModifierCoefficient
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories = "Stat"))
	FGameplayTag Stat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, float> Coefficients;
};

// Set modifiers from table
USTRUCT(BlueprintType)
struct FRsEffectModifierTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories = "Stat"))
	FGameplayTag Stat1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Expression1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories = "Stat"))
	FGameplayTag Stat2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Expression2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories = "Stat"))
	FGameplayTag Stat3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Expression3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories = "Stat"))
	FGameplayTag Stat4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Expression4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories = "Stat"))
	FGameplayTag Stat5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Expression5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories = "Stat"))
	FGameplayTag Stat6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Expression6;
};

/**
 * 
 */
UCLASS()
class RS_API URsModifierDataEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FModifierCoefficient> ModifierCoefficients;

	UPROPERTY(EditDefaultsOnly, meta = (RowType = "RsEffectModifierTableRow"))
	FDataTableRowHandle DataTableRow;

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty( struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
};
