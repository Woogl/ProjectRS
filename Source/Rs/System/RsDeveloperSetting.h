// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "RsDeveloperSetting.generated.h"

class UAttributeSet;
/**
 * 
 */
UCLASS()
class URsDeveloperSetting : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	static const URsDeveloperSetting* Get();
	
	// UDeveloperSettings interface
	virtual FName GetCategoryName() const override;
};
