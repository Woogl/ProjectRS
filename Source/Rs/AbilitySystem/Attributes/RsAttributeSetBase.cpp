// Copyright 2024 Team BH.


#include "RsAttributeSetBase.h"

#include "Rs/RsLogChannels.h"

FGameplayAttribute URsAttributeSetBase::TagToAttribute(const FGameplayTag& Tag)
{
	if (FGameplayAttribute* Found = StatMap.Find(Tag))
	{
		return *Found;
	}
	for (const TTuple<FGameplayTag, FGameplayAttribute>& Coefficient : CoefficientMap)
	{
		if (Coefficient.Key.MatchesTag(Tag))
		{
			return Coefficient.Value;
		}
	}
	UE_LOG(RsAbilityLog, Warning, TEXT("Cannot find Attribute for [%s] tag"), *Tag.ToString());
	return FGameplayAttribute();
}

const TMap<FGameplayTag, FGameplayAttribute>& URsAttributeSetBase::GetStatMap()
{
	return StatMap;
}

const TMap<FGameplayTag, FGameplayAttribute>& URsAttributeSetBase::GetCoefficientMap()
{
	return CoefficientMap;
}

void URsAttributeSetBase::AdjustAttributeForMaxChange(const FGameplayAttribute& AffectedAttribute, float OldMaxValue, float NewMaxValue) const
{
	UAbilitySystemComponent* const ASC = GetOwningAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}
	
	if (OldMaxValue <= 0.f || FMath::IsNearlyEqual(OldMaxValue, NewMaxValue, 0.f))
	{
		return;
	}
	
	// Change current value to maintain the Current Value / Maximum Value percentage.
	ASC->SetNumericAttributeBase(AffectedAttribute, ASC->GetNumericAttributeBase(AffectedAttribute) * NewMaxValue / OldMaxValue);
}

void URsAttributeSetBase::RegisterTagToStat(const FGameplayTag& Tag, FGameplayAttribute Attribute)
{
	StatMap.Add(Tag, Attribute);
}

void URsAttributeSetBase::RegisterTagToCoefficient(const FGameplayTag& Tag, FGameplayAttribute Attribute)
{
	CoefficientMap.Add(Tag, Attribute);
}
