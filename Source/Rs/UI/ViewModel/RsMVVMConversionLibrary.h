// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Bindings/MVVMConversionLibrary.h"
#include "RsMVVMConversionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsMVVMConversionLibrary : public UMVVMConversionLibrary
{
	GENERATED_BODY()

	// Show 1 decimal place in the resulting text. (12.345 -> 12.3)
	UFUNCTION(BlueprintPure, Category = "Widget", meta = (DisplayName = "Cooldown Time (Float -> Text)"))
	static FText CooldownTimeToText(float CooldownTime);
};
