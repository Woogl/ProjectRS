// Copyright 2025 Team BH.


#include "RsAbilitySystemSettings.h"

const URsAbilitySystemSettings& URsAbilitySystemSettings::Get()
{
	return *GetDefault<URsAbilitySystemSettings>();
}

FName URsAbilitySystemSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}

FGameplayAttribute URsAbilitySystemSettings::FindAttributeFromCoefficientTag(const FGameplayTag& InTag) const
{
	for (const TTuple<FGameplayTag, FGameplayAttribute>& Coeff : Coefficients)
	{
		if (InTag.MatchesTag(Coeff.Key))
		{
			return Coeff.Value;
		}
	}
	return FGameplayAttribute();
}
