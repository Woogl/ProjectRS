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

FString FRsEffectTableRow::FindValue(FName Key) const
{
	if (Key == GET_MEMBER_NAME_CHECKED(FRsEffectTableRow, Key1))
	{
		return Value1;
	}
	if (Key == GET_MEMBER_NAME_CHECKED(FRsEffectTableRow, Key2))
	{
		return Value2;
	}
	if (Key == GET_MEMBER_NAME_CHECKED(FRsEffectTableRow, Key3))
	{
		return Value3;
	}
	if (Key == GET_MEMBER_NAME_CHECKED(FRsEffectTableRow, Key4))
	{
		return Value4;
	}
	if (Key == GET_MEMBER_NAME_CHECKED(FRsEffectTableRow, Key5))
	{
		return Value5;
	}
	if (Key == GET_MEMBER_NAME_CHECKED(FRsEffectTableRow, Key6))
	{
		return Value6;
	}
	if (Key == GET_MEMBER_NAME_CHECKED(FRsEffectTableRow, Key7))
	{
		return Value7;
	}
	if (Key == GET_MEMBER_NAME_CHECKED(FRsEffectTableRow, Key8))
	{
		return Value8;
	}
	if (Key == GET_MEMBER_NAME_CHECKED(FRsEffectTableRow, Key9))
	{
		return Value9;
	}
	UE_LOG(RsLog, Warning, TEXT("Cannot find [ %s ] value in RsEffectTableRow."), *Key.ToString());
	return FString();
}

