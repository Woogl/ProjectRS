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
UCLASS(Config=Game, DefaultConfig)
class RS_API URsAbilitySystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	const static URsAbilitySystemSettings& Get();
	
	// UDeveloperSettings interface
	virtual FName GetCategoryName() const override;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Ability")
	TSubclassOf<UGameplayEffect> DefaultCostEffect;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Ability")
	TSubclassOf<UGameplayEffect> DefaultCooldownEffect;
	
	UPROPERTY(EditDefaultsOnly, Config, Category = "Effect")
	TArray<TSoftObjectPtr<UDataTable>> EffectTables;
	
	UPROPERTY(EditDefaultsOnly, Config, Category = "Battle")
	float DamageReductionConstant = 190.f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Battle")
	float GroggyDamageMultiplier = 1.6f;
};
