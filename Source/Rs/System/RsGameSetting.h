// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RsGameSetting.generated.h"

/**
 * Golbal game setting datas for Project RS
 * Datas in this class is always loaded in memory.
 */
UCLASS()
class RS_API URsGameSetting : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	static const URsGameSetting* Get();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Ability")
	FGameplayTag DeathAbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Ability")
	FGameplayTag GroggyAbilityTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Ability")
	FGameplayTag ESkillTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Ability")
	FGameplayTag QSkillTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Ability")
	FGameplayTag UltSkillTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Ability")
	FGameplayTag LinkSkillTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Effect")
	FGameplayTag SwitchMemberCooldownTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle")
	float DefenseConstant = 190.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle")
	float GroggyDamageMultiplier = 1.6f;
};
