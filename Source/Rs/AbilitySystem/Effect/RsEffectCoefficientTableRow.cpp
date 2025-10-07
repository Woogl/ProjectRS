// Copyright 2025 Team BH.


#include "RsEffectCoefficientTableRow.h"

#include "RsUnitEffect.h"
#include "Misc/DataValidation.h"

#if WITH_EDITOR
EDataValidationResult FRsEffectCoefficientTableRow::IsDataValid(FDataValidationContext& Context) const
{
	auto CheckTableRow = [&](const TSubclassOf<URsUnitEffect>& EffectClass, const FString& Expression, int32 Index)
	{
		bool bEffectSet = EffectClass != nullptr;
		bool bExpressionSet = !Expression.IsEmpty();
		if (bEffectSet != bExpressionSet)
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("UnitEffect%d and CoefficientExpression%d must both be set or both be empty."), Index, Index)));
			return false;
		}
		return true;
	};

	if (!CheckTableRow(UnitEffect1, CoefficientExpression1, 1) ||
		!CheckTableRow(UnitEffect2, CoefficientExpression2, 2) ||
		!CheckTableRow(UnitEffect3, CoefficientExpression3, 3) ||
		!CheckTableRow(UnitEffect4, CoefficientExpression4, 4) ||
		!CheckTableRow(UnitEffect5, CoefficientExpression5, 5) ||
		!CheckTableRow(UnitEffect6, CoefficientExpression6, 6))
	{
		return EDataValidationResult::Invalid;
	}
	
	return EDataValidationResult::Valid;
}
#endif // WITH_EDITOR
