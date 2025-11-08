// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RsDeveloperSetting.generated.h"

class URsGameSettingDataAsset;
/**
 * 
 */
UCLASS(Config=Game)
class URsDeveloperSetting : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	static const URsDeveloperSetting& Get();
	
	// UDeveloperSettings interface
	virtual FName GetCategoryName() const override;
	
	UPROPERTY(EditDefaultsOnly, config, Category = "Data Asset")
	TSoftObjectPtr<URsGameSettingDataAsset> GameSettingDataAsset;
};
