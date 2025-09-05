// Copyright 2024 Team BH.


#include "RsMVVMConversionLibrary.h"

FText URsMVVMConversionLibrary::FloatToText(float Number, int32 FractionDigits)
{
	static const FNumberFormattingOptions Format = FNumberFormattingOptions().SetMinimumFractionalDigits(FractionDigits).SetMaximumFractionalDigits(FractionDigits);
	return FText::AsNumber(Number, &Format);
}

FText URsMVVMConversionLibrary::IntegerToText(int32 Number)
{
	return FText::AsNumber(Number);
}

FText URsMVVMConversionLibrary::SecondToTimer(float Second)
{
	int32 TotalMilliSeconds = FMath::RoundToInt(Second * 1000.0f);
	int32 Minutes = (TotalMilliSeconds / 1000) / 60;
	int32 Seconds = (TotalMilliSeconds / 1000) % 60;
	int32 CentiSeconds = (TotalMilliSeconds % 1000) / 10;
	return FText::FromString(FString::Printf(TEXT("%02d:%02d:%02d"), Minutes, Seconds, CentiSeconds));
}
