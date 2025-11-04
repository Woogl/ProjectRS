// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"
#include "RsModifierDataEffectComponent.generated.h"

// Used for setting RsGameplayEffect's modifiers from asset.
USTRUCT(BlueprintType)
struct FModifierCoefficient
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories = "Stat,Meta"))
	FGameplayTag Stat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, float> Coefficients;
};

// Used for setting RsGameplayEffect's modifiers from table.
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
 * When RsGameplayEffect asset is saved, this component automatically updates its modifiers.
 */
UCLASS()
class RS_API URsModifierDataEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	URsModifierDataEffectComponent();
	
#if WITH_EDITORONLY_DATA
	// Data for setting RsGameplayEffect's modifiers from asset.
	UPROPERTY(EditDefaultsOnly)
	TArray<FModifierCoefficient> ModifierCoefficients;

	// Data for setting RsGameplayEffect's modifiers from table.
	UPROPERTY(EditDefaultsOnly, meta = (RowType = "RsEffectModifierTableRow"))
	FDataTableRowHandle DataTableRow;
#endif // WITH_EDITORONLY_DATA

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty( struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
};
