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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag DeathAbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag GroggyAbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag ESkillTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag QSkillTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag UltSkillTag;

	// NOTE: MVVM settings are not saved when this file is set in DefaultGame.ini
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCommonActivatableWidget> GameHUDClass;
};
