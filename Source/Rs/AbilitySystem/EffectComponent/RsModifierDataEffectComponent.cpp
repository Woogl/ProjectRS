// Copyright 2025 Team BH.


#include "RsModifierDataEffectComponent.h"

#include "Misc/DataValidation.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffect.h"

URsModifierDataEffectComponent::URsModifierDataEffectComponent()
{
}

#if WITH_EDITOR
EDataValidationResult URsModifierDataEffectComponent::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	// Check GE
	if (GetOuter() && !GetOuter()->IsA(URsGameplayEffect::StaticClass()))
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("RsModifierDataEffectComponent supports RsGameplayEffect only."))));
		return EDataValidationResult::Invalid;
	}

	// Check asset data
	for (const FModifierCoefficient& ModCoeff : ModifierCoefficients)
	{
		for (const auto [CoeffTag, CoeffNum] : ModCoeff.Coefficients)
		{
			if (CoeffTag == RsGameplayTags::COEFFICIENT_CONSTANT)
			{
				continue;
			}
			FString CoeffTagString = CoeffTag.ToString();
			if (CoeffTagString.IsEmpty())
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag is empty"))));
				return EDataValidationResult::Invalid;
			}
			if (!CoeffTagString.StartsWith(TEXT("Coefficient.")))
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag \"%s\" must start with \"Coefficient.\""), *CoeffTag.ToString())));
				return EDataValidationResult::Invalid;
			}
			if (!CoeffTagString.EndsWith(TEXT(".Source")) && !CoeffTagString.EndsWith(TEXT(".Target")))
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag \"%s\" must end with \".Target\" or \".Source\""), *CoeffTag.ToString())));
				return EDataValidationResult::Invalid;
			}
		}
	}
	return Result;
}
#endif // WITH_EDITOR
