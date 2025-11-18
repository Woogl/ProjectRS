// Copyright 2025 Team BH.


#include "RsEffectTable.h"

#include "GameplayEffect.h"
#include "Misc/DataValidation.h"

#if WITH_EDITOR
EDataValidationResult FRsEffectTableRowBase::IsDataValid(FDataValidationContext& Context) const
{
	if (!EffectClass || !EffectClass->IsChildOf(UGameplayEffect::StaticClass()))
	{
		FString EffectString = EffectClass ? EffectClass->GetName() : TEXT("NULL");
		FText EffectText = FText::FromString(EffectString);
		Context.AddError(FText::Format(NSLOCTEXT("RsEffectTable", "InvalidEffectClass", "EffectClass is not valid. {0}"), EffectText));
		return EDataValidationResult::Invalid;
	}
	return FTableRowBase::IsDataValid(Context);
}
#endif // WITH_EDITOR
