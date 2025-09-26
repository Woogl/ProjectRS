// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"
#include "RsUnitEffectComponent.generated.h"

class URsUnitEffect;
struct FRsEffectCoefficient;
/**
 * 
 */
UCLASS()
class RS_API URsUnitEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URsUnitEffect> Effect;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, float> Coefficients;

	UPROPERTY(EditDefaultsOnly, meta = (RowType = "RsEffectCoefficientTableRow"))
	FDataTableRowHandle DataTableRow;
	
public:
#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty( struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
	
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

protected:
	FRsEffectCoefficient GetEffectCoefficient() const;
};
