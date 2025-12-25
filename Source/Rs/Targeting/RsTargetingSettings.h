// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RsTargetingSettings.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsTargetingSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	const static URsTargetingSettings& Get();
	
	// UDeveloperSettings interface
	virtual FName GetCategoryName() const override;
};
