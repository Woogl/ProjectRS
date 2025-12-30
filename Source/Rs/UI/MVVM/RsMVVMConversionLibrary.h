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
	
public:
	UFUNCTION(BlueprintPure, Category = "RS | ViewModel", meta = (DisplayName = "Float -> Text"))
	static FText FloatToText(float Number, int32 FractionDigits);
	
	UFUNCTION(BlueprintPure, Category = "RS | ViewModel", meta = (DisplayName = "Int -> Text"))
	static FText IntegerToText(int32 Number);

	UFUNCTION(BlueprintPure, Category = "RS | ViewModel", meta = (DisplayName = "Float -> Text (MM:SS:MS)"))
	static FText SecondToTimeText(float Second);
};
