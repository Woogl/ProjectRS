// Copyright 2025 Team BH.


#include "RsUnitEffectsComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Misc/DataValidation.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficient.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficientTableRow.h"
#include "Rs/AbilitySystem/Effect/RsUnitEffect.h"

#if WITH_EDITOR
bool URsUnitEffectsComponent::CanEditChange(const FProperty* InProperty) const
{
	bool bParentVal = Super::CanEditChange(InProperty);
	
	bool bDataTableValid = !DataTableRow.IsNull();
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, EffectCoefficients))
	{
		return bParentVal && !bDataTableValid;
	}
	return bParentVal;
}

void URsUnitEffectsComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName MemberPropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, DataTableRow))
	{
		EffectCoefficients.Empty();
	}
}

EDataValidationResult URsUnitEffectsComponent::IsDataValid(class FDataValidationContext& Context) const
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

	Result = Row->IsDataValid(Context);
	if (Result == EDataValidationResult::Invalid)
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid DataTable row. %s"), *DataTableRow.ToDebugString())));
	}
	
	return Result;
}
#endif // WITH_EDITOR

void URsUnitEffectsComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	if (!ensureMsgf(ActiveGEContainer.Owner, TEXT("OnGameplayEffectApplied is passed an ActiveGEContainer which lives within an ASC but that ASC was somehow null")))
	{
		return;
	}

	TArray<FRsEffectCoefficient> EffectCoeffs = GetEffectCoefficients();
	UAbilitySystemComponent* SourceASC = GESpec.GetContext().GetInstigatorAbilitySystemComponent();

	for (const FRsEffectCoefficient& EffectCoeff : EffectCoeffs)
	{
		FGameplayEffectSpecHandle EffectHandle = URsAbilitySystemLibrary::MakeEffectSpecCoefficient(SourceASC, EffectCoeff, SourceASC->MakeEffectContext());
		// Duration is shared from owning GE.
		EffectHandle.Data->SetSetByCallerMagnitude(RsGameplayTags::DURATION, GESpec.Duration);

		URsAbilitySystemLibrary::ApplyEffectSpecCoefficient(SourceASC, ActiveGEContainer.Owner, EffectHandle);
	}
}

TArray<FRsEffectCoefficient> URsUnitEffectsComponent::GetEffectCoefficients() const
{
	if (DataTableRow.IsNull())
	{
		return EffectCoefficients;
	}

	TArray<FRsEffectCoefficient> OutEffectCoeffs;
	if (FRsEffectCoefficientTableRow* Row = DataTableRow.GetRow<FRsEffectCoefficientTableRow>(ANSI_TO_TCHAR(__FUNCTION__)))
	{
		if (Row->UnitEffect1 && !Row->CoefficientExpression1.IsEmpty())
		{
			FRsEffectCoefficient EffectCoeff1 = MakeEffectCoefficientFromTable(Row->UnitEffect1, Row->CoefficientExpression1);
			OutEffectCoeffs.Add(EffectCoeff1);
		}
		if (Row->UnitEffect2 && !Row->CoefficientExpression2.IsEmpty())
		{
			FRsEffectCoefficient EffectCoeff2 = MakeEffectCoefficientFromTable(Row->UnitEffect2, Row->CoefficientExpression2);
			OutEffectCoeffs.Add(EffectCoeff2);
		}
		if (Row->UnitEffect3 && !Row->CoefficientExpression3.IsEmpty())
		{
			FRsEffectCoefficient EffectCoeff3 = MakeEffectCoefficientFromTable(Row->UnitEffect3, Row->CoefficientExpression3);
			OutEffectCoeffs.Add(EffectCoeff3);
		}
		if (Row->UnitEffect4 && !Row->CoefficientExpression4.IsEmpty())
		{
			FRsEffectCoefficient EffectCoeff4 = MakeEffectCoefficientFromTable(Row->UnitEffect4, Row->CoefficientExpression4);
			OutEffectCoeffs.Add(EffectCoeff4);
		}
		if (Row->UnitEffect5 && !Row->CoefficientExpression5.IsEmpty())
		{
			FRsEffectCoefficient EffectCoeff5 = MakeEffectCoefficientFromTable(Row->UnitEffect5, Row->CoefficientExpression5);
			OutEffectCoeffs.Add(EffectCoeff5);
		}
		if (Row->UnitEffect6 && !Row->CoefficientExpression6.IsEmpty())
		{
			FRsEffectCoefficient EffectCoeff6 = MakeEffectCoefficientFromTable(Row->UnitEffect6, Row->CoefficientExpression6);
			OutEffectCoeffs.Add(EffectCoeff6);
		}
		return OutEffectCoeffs;
	}

	UE_LOG(RsLog, Error, TEXT("%s"), *DataTableRow.ToDebugString());
	return TArray<FRsEffectCoefficient>();
}

FRsEffectCoefficient URsUnitEffectsComponent::MakeEffectCoefficientFromTable(TSubclassOf<UGameplayEffect> EffectClass, FString CoefficientExpression) const
{
	FRsEffectCoefficient EffectCoefficient;
	EffectCoefficient.EffectClass = EffectClass;
	// TODO: Parse coefficient expression
	// EffectCoefficient.Coefficients = ParseExpression(CoefficientExpression);
	return EffectCoefficient;
}
