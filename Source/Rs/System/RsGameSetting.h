// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RsGameSetting.generated.h"

/**
 * 
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle")
	FGameplayTag DamageCoefficientTag;
};
