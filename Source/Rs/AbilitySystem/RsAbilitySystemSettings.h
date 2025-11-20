// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "RsAbilitySystemSettings.generated.h"

class UGameplayEffect;
class URsAbilitySet;
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
	
	UPROPERTY(EditDefaultsOnly, config, Category = "Attribute", meta = (Categories = "Stat,Meta", ForceInlineRow))
	TMap<FGameplayTag, FGameplayAttribute> Attributes;

	UPROPERTY(EditDefaultsOnly, config, Category = "Attribute", meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, FGameplayAttribute> Coefficients;

	UPROPERTY(EditDefaultsOnly, config, Category = "Ability")
	TSubclassOf<UGameplayEffect> DefaultCostEffect;

	UPROPERTY(EditDefaultsOnly, config, Category = "Ability")
	TSubclassOf<UGameplayEffect> DefaultCooldownEffect;

	UPROPERTY(EditDefaultsOnly, config, Category = "Effect")
	TArray<TSoftObjectPtr<UDataTable>> SharedEffectTables;
	
	// Data used to initialize the environmental effects source.
	UPROPERTY(EditDefaultsOnly, config, Category = "Environments")
	TSoftObjectPtr<URsAbilitySet> WorldAbilitySet;
};
