// Copyright 2025 Team BH.


#include "RsEffectTable.h"

#include "GameplayEffect.h"
#include "Misc/DataValidation.h"
#include "Rs/RsLogChannels.h"

#if WITH_EDITOR
EDataValidationResult FRsEffectTableRowBase::IsDataValid(FDataValidationContext& Context) const
{
	if (!EffectClass || !EffectClass->IsChildOf(UGameplayEffect::StaticClass()))
	{
		FString EffectString = EffectClass ? EffectClass->GetName() : TEXT("NULL");
		FString ErrorString = FString::Printf(TEXT("EffectClass [ %s ] is not a child of UGameplayEffect."), *EffectString);
		Context.AddError(FText::FromString(ErrorString));
		return EDataValidationResult::Invalid;
	}
	return FTableRowBase::IsDataValid(Context);
}

EDataValidationResult FRsEffectTableRow::IsDataValid(FDataValidationContext& Context) const
{
	TArray<FName> AllKeys = { Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9 };
	TSet<FName> SeenKeys;

	bool bIsAlreadyInSetPtr;
	for (const FName& Key : AllKeys)
	{
		if (Key.IsNone())
		{
			continue;
		}
		
		SeenKeys.Add(Key, &bIsAlreadyInSetPtr);
		if (bIsAlreadyInSetPtr)
		{
			FString ErrorString = FString::Printf(TEXT("Duplicate key name [ %s ] detected."), *Key.ToString());
			Context.AddError(FText::FromString(ErrorString));
			return EDataValidationResult::Invalid;
		}
	}

	return FRsEffectTableRowBase::IsDataValid(Context);
}
#endif // WITH_EDITOR

FString FRsEffectTableRow::FindValueInternal(FName Key, bool bWarnIfNotFound) const
{
	if (Key == Key1)
	{
		return Value1;
	}
	if (Key == Key2)
	{
		return Value2;
	}
	if (Key == Key3)
	{
		return Value3;
	}
	if (Key == Key4)
	{
		return Value4;
	}
	if (Key == Key5)
	{
		return Value5;
	}
	if (Key == Key6)
	{
		return Value6;
	}
	if (Key == Key7)
	{
		return Value7;
	}
	if (Key == Key8)
	{
		return Value8;
	}
	if (Key == Key9)
	{
		return Value9;
	}
	if (bWarnIfNotFound)
	{
		UE_LOG(RsLog, Warning, TEXT("Cannot find [ %s ] value in RsEffectTableRow."), *Key.ToString());
	}
	return FString();
}

template <>
FName FRsEffectTableRow::FindValue<FName>(FName Key, bool bWarnIfNotFound) const
{
	FString StringValue = FindValueInternal(Key, bWarnIfNotFound);
	return FName(StringValue);
}

template <>
FString FRsEffectTableRow::FindValue<FString>(FName Key, bool bWarnIfNotFound) const
{
	FString StringValue = FindValueInternal(Key, bWarnIfNotFound);
	return StringValue;
}

template <>
int32 FRsEffectTableRow::FindValue<int32>(FName Key, bool bWarnIfNotFound) const
{
	FString StringValue = FindValueInternal(Key, bWarnIfNotFound);
	return FCString::Atoi(*StringValue);
}

template <>
float FRsEffectTableRow::FindValue<float>(FName Key, bool bWarnIfNotFound) const
{
	FString StringValue = FindValueInternal(Key, bWarnIfNotFound);
	return FCString::Atof(*StringValue);
}

template <>
bool FRsEffectTableRow::FindValue<bool>(FName Key, bool bWarnIfNotFound) const
{
	FString StringValue = FindValueInternal(Key, bWarnIfNotFound);
	return StringValue.ToBool();
}