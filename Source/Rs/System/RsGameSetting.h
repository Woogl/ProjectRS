// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RsGameSetting.generated.h"

class UCommonActivatableWidget;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Effect")
	FGameplayTag SwitchMemberCooldownTag;

	// NOTE: MVVM settings are not saved when this file is set in DefaultGame.ini
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCommonActivatableWidget> GameHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCommonActivatableWidget> TripleLinkSkillWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle")
	float DefenseConstant = 190.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle")
	float GroggyDamageMultiplier = 1.6f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle")
	float TripleLinkSkillDuration = 3.f;
};
