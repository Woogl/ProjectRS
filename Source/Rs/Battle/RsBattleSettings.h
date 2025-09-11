// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "RsBattleSettings.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS(Config=Game)
class RS_API URsBattleSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	const static URsBattleSettings& Get();
	
	// UDeveloperSettings interface
	virtual FName GetCategoryName() const override;

	UPROPERTY(EditDefaultsOnly, config, Category = "Effect", meta = (Categories = "Stat", ForceInlineRow))
	TMap<FGameplayTag, FGameplayAttribute> TaggedStats;

	UPROPERTY(EditDefaultsOnly, config, Category = "Effect", meta = (Categories = "Stat", ForceInlineRow))
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> BuffEffects;
};
