// Copyright 2024 Team BH.


#include "RsMVVMConversionLibrary.h"

FText URsMVVMConversionLibrary::FloatToText(float Number, int32 FractionDigits)
{
	return FText::AsNumber(Number, &FNumberFormattingOptions().SetMinimumFractionalDigits(FractionDigits).SetMaximumFractionalDigits(FractionDigits));
}

FText URsMVVMConversionLibrary::IntegerToText(int32 Number)
{
	return FText::AsNumber(Number);
}
