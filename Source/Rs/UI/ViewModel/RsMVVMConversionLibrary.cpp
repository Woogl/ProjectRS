// Copyright 2024 Team BH.


#include "RsMVVMConversionLibrary.h"

FText URsMVVMConversionLibrary::FloatToText(float Number, int32 FractionDigits)
{
	static const FNumberFormattingOptions Format = FNumberFormattingOptions().SetMinimumFractionalDigits(1).SetMaximumFractionalDigits(1);
	return FText::AsNumber(Number, &Format);
}

FText URsMVVMConversionLibrary::IntegerToText(int32 Number)
{
	return FText::AsNumber(Number);
}
