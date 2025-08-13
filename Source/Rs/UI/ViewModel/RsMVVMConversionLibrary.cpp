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

FText URsMVVMConversionLibrary::SecondToTimer(float Second)
{
	int32 TotalMilliseconds = FMath::RoundToInt(Second * 1000.0f);
	int32 Minutes = (TotalMilliseconds / 1000) / 60;
	int32 Seconds = (TotalMilliseconds / 1000) % 60;
	int32 Centiseconds = (TotalMilliseconds % 1000) / 10;
	return FText::FromString(FString::Printf(TEXT("%02d:%02d:%02d"), Minutes, Seconds, Centiseconds));
}
