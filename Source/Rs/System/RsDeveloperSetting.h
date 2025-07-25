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

	UPROPERTY(EditDefaultsOnly, config, Category = "Effect | Damage | Instant")
	TSubclassOf<UGameplayEffect> HealthDamageEffectClass;

	UPROPERTY(EditDefaultsOnly, config, Category = "Effect | Damage | Instant")
	TSubclassOf<UGameplayEffect> StaggerDamageEffectClass;

	UPROPERTY(EditDefaultsOnly, config, Category = "Effect | Damage | DoT")
	TSubclassOf<UGameplayEffect> HealthDotDamageEffectClass;

	UPROPERTY(EditDefaultsOnly, config, Category = "Effect | Damage | DoT")
	TSubclassOf<UGameplayEffect> StaggerDotDamageEffectClass;

	UPROPERTY(EditDefaultsOnly, config, Category = "Effect | Damage | DoT Burst")
	TSubclassOf<UGameplayEffect> DotBurstDamageEffectClass;

	UPROPERTY(EditDefaultsOnly, config, Category = "Effect | Damage | Hit Reaction", meta = (Categories = "Ability.HitReaction", ForceInlineRow))
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> HitReactionEffectClasses;
};
