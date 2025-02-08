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
	
	UFUNCTION(BlueprintCallable)
	static URsGameSetting* Get();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag HealthDamageCoefficientTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag StaggerDamageCoefficientTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag HitReactionAbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag DeathAbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag StunAbilityTag;
};
