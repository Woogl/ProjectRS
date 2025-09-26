// Copyright 2025 Team BH.


#include "RsUnitEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Misc/DataValidation.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficient.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficientTableRow.h"
#include "Rs/AbilitySystem/Effect/RsUnitEffect.h"

#if WITH_EDITOR
bool URsUnitEffectComponent::CanEditChange(const FProperty* InProperty) const
{
	bool bParentVal = Super::CanEditChange(InProperty);
	
	bool bDataTableValid = !DataTableRow.IsNull();
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, Coefficients))
	{
		return bParentVal && !bDataTableValid;
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, Effect))
	{
		return bParentVal && !bDataTableValid;
	}
	return bParentVal;
}

void URsUnitEffectComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName MemberPropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, DataTableRow))
	{
		Effect = nullptr;
		Coefficients.Empty();
	}
}

EDataValidationResult URsUnitEffectComponent::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (DataTableRow.IsNull())
	{
		return Result;
	}

	const FRsEffectCoefficientTableRow* Row = DataTableRow.GetRow<FRsEffectCoefficientTableRow>(TEXT(__FUNCTION__));
	if (!Row)
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid DataTable row. %s"), *DataTableRow.ToDebugString())));
		return EDataValidationResult::Invalid;
	}

	if (!Row->EffectClass)
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("EffectClass is not set. %s"), *DataTableRow.ToDebugString())));
		return EDataValidationResult::Invalid;
	}
	
	return Result;
}
#endif // WITH_EDITOR

void URsUnitEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	if (!ensureMsgf(ActiveGEContainer.Owner, TEXT("OnGameplayEffectApplied is passed an ActiveGEContainer which lives within an ASC but that ASC was somehow null")))
	{
		return;
	}
	
	FRsEffectCoefficient EffectCoefficient = GetEffectCoefficient();
	UAbilitySystemComponent* SourceASC = GESpec.GetContext().GetInstigatorAbilitySystemComponent();

	FGameplayEffectSpecHandle EffectHandle = URsAbilitySystemLibrary::MakeEffectSpecCoefficient(SourceASC, EffectCoefficient, SourceASC->MakeEffectContext());
	// Duration is shared from owning GE.
	EffectHandle.Data->SetDuration(GESpec.Duration, true);

	URsAbilitySystemLibrary::ApplyEffectSpecCoefficient(SourceASC, ActiveGEContainer.Owner, EffectHandle);
}

FRsEffectCoefficient URsUnitEffectComponent::GetEffectCoefficient() const
{
	if (DataTableRow.IsNull())
	{
		return FRsEffectCoefficient(Effect, Coefficients);
	}
	
	if (FRsEffectCoefficientTableRow* Row = DataTableRow.GetRow<FRsEffectCoefficientTableRow>(ANSI_TO_TCHAR(__FUNCTION__)))
	{
		TMap<FGameplayTag, float> Coeff_Table;
		if (Row->Tag1.IsValid())
		{
			Coeff_Table.Add(Row->Tag1, Row->Value1);
		}
		if (Row->Tag2.IsValid())
		{
			Coeff_Table.Add(Row->Tag2, Row->Value2);
		}
		if (Row->Tag3.IsValid())
		{
			Coeff_Table.Add(Row->Tag3, Row->Value3);
		}
		return FRsEffectCoefficient(Row->EffectClass, Coeff_Table);
	}

	UE_LOG(RsLog, Error, TEXT("%s"), *DataTableRow.ToDebugString());
	return FRsEffectCoefficient();
}
