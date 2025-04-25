// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "RsDeveloperSetting.generated.h"

class UGameplayEffect;
class UAttributeSet;
/**
 * 
 */
UCLASS(Config=Game)
class URsDeveloperSetting : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	static const URsDeveloperSetting* Get();
	
	// UDeveloperSettings interface
	virtual FName GetCategoryName() const override;

	UPROPERTY(EditDefaultsOnly, config, Category = "Effect | Magnitude", meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, FGameplayAttribute> CoefficientTags;

	UPROPERTY(EditDefaultsOnly, config, Category = "Effect | Magnitude", meta = (Categories = "Manual"))
	FGameplayTag ManualMagnitudeTag;
	
	UPROPERTY(EditDefaultsOnly, config, Category = "Effect | Magnitude", meta = (Categories = "Manual"))
	FGameplayTag ManualDurationTag;
	
	UPROPERTY(EditDefaultsOnly, config, Category = "Effect | Magnitude", meta = (Categories = "Manual"))
	FGameplayTag ManualLevelTag;

	UPROPERTY(EditDefaultsOnly, config, Category = "Effect | Damage")
	TArray<TSubclassOf<UGameplayEffect>> DamageEffectApplicationOrder;
};
