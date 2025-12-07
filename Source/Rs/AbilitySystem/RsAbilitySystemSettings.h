// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(EditDefaultsOnly, config, Category = "Ability")
	TSubclassOf<UGameplayEffect> DefaultCostEffect;

	UPROPERTY(EditDefaultsOnly, config, Category = "Ability")
	TSubclassOf<UGameplayEffect> DefaultCooldownEffect;
};
