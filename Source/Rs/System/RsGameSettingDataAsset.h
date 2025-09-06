// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RsGameSettingDataAsset.generated.h"

/**
 * Golbal game setting data asset for designer.
 */
UCLASS()
class RS_API URsGameSettingDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	static const URsGameSettingDataAsset* Get();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | UI")
	FGameplayTag PauseMenuTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | UI")
	FGameplayTag TripleLinkSkillTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle")
	float DefenseConstant = 190.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle")
	float GroggyDamageMultiplier = 1.6f;
};
