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
	UPROPERTY(EditDefaultsOnly)
	TArray<FModifierCoefficient> ModifierCoefficients;
#endif // WITH_EDITORONLY_DATA

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
};
