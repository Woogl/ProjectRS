// Copyright 2025 Team BH.


#include "RsModifierDataEffectComponent.h"

#include "Misc/DataValidation.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffect.h"

#if WITH_EDITOR
bool URsModifierDataEffectComponent::CanEditChange(const FProperty* InProperty) const
{
	bool bParentVal = Super::CanEditChange(InProperty);
	
	bool bDataTableValid = !DataTableRow.IsNull();
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, ModifierCoefficients))
	{
		return bParentVal && !bDataTableValid;
	}
	return bParentVal;
}

void URsModifierDataEffectComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName MemberPropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, DataTableRow))
	{
		ModifierCoefficients.Empty();
	}
}

EDataValidationResult URsModifierDataEffectComponent::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	// Check GE
	if (GetOuter() && !GetOuter()->IsA(URsGameplayEffect::StaticClass()))
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("RsModifierDataEffectComponent only support RsGameplayEffect."))));
		return EDataValidationResult::Invalid;
	}

	// Check asset data
	if (DataTableRow.IsNull())
	{
		for (const FModifierCoefficient& ModCoeff : ModifierCoefficients)
		{
			for (const auto [CoeffTag, CoeffNum] : ModCoeff.Coefficients)
			{
				if (CoeffTag == RsGameplayTags::COEFFICIENT_MANUAL)
				{
					continue;
				}
				FString CoeffTagString = CoeffTag.ToString();
				if (!CoeffTagString.EndsWith(TEXT(".Source")) && !CoeffTagString.EndsWith(TEXT(".Target")))
				{
					Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag { %s } should end with \".Target\" or \".Source\"."), *CoeffTag.ToString())));
					return EDataValidationResult::Invalid;
				}
			}
		}
		return Result;
	}

	// Check table data
	const FRsEffectModifierTableRow* Row = DataTableRow.GetRow<FRsEffectModifierTableRow>(TEXT(__FUNCTION__));
	if (!Row)
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid DataTable row. %s"), *DataTableRow.ToDebugString())));
		return EDataValidationResult::Invalid;
	}
	if (Row->IsDataValid(Context) == EDataValidationResult::Invalid)
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid DataTable row. %s"), *DataTableRow.ToDebugString())));
		return EDataValidationResult::Invalid;
	}
	
	return Result;
}
#endif // WITH_EDITOR
