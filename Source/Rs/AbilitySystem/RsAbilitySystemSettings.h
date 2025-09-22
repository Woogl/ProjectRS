// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "RsAbilitySystemSettings.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS(Config=Game)
class RS_API URsAbilitySystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	const static URsAbilitySystemSettings& Get();
	
	// UDeveloperSettings interface
	virtual FName GetCategoryName() const override;
	
	UPROPERTY(EditDefaultsOnly, config, meta = (Categories = "Stat", ForceInlineRow))
	TMap<FGameplayTag, FGameplayAttribute> Attributes;

	UPROPERTY(EditDefaultsOnly, config, meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, FGameplayAttribute> Coefficients;
	
	UPROPERTY(EditDefaultsOnly, config, meta = (Categories = "Stat", ForceInlineRow))
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> BuffEffects;
};
