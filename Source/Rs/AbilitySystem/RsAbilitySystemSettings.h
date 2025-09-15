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

	// TODO: remove coefficient tags
	UPROPERTY(EditDefaultsOnly, config, Category = "Effect", meta = (Categories = "Stat,Coefficient", ForceInlineRow))
	TMap<FGameplayTag, FGameplayAttribute> TaggedStats;

	// TODO: remove coefficient tags
	UPROPERTY(EditDefaultsOnly, config, Category = "Effect", meta = (Categories = "Stat,Coefficient", ForceInlineRow))
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> BuffEffects;
};
