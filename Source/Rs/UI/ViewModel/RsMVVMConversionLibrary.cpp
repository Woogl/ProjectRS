// Copyright 2024 Team BH.


#include "RsMVVMConversionLibrary.h"

FText URsMVVMConversionLibrary::CooldownTimeToText(float CooldownTime)
{
	int32 Truncated = static_cast<int32>(CooldownTime * 10);
	float DisplayValue = Truncated / 10.f;
	return FText::AsNumber(DisplayValue, &FNumberFormattingOptions().SetMaximumFractionalDigits(1).SetMinimumFractionalDigits(1));
}
