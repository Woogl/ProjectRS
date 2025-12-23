// Copyright 2024 Team BH.


#include "RsAttributeSetBase.h"

#include "Rs/RsLogChannels.h"

FGameplayAttribute URsAttributeSetBase::TagToAttribute(const FGameplayTag& Tag)
{
	if (FGameplayAttribute* Found = TagStatMap.Find(Tag))
	{
		return *Found;
	}
	for (const TTuple<FGameplayTag, FGameplayAttribute>& Coefficient : TagCoefficientMap)
	{
		if (Coefficient.Key.MatchesTag(Tag))
		{
			return Coefficient.Value;
		}
	}
	UE_LOG(LogRsAbility, Warning, TEXT("Cannot find Attribute for [%s] Tag"), *Tag.ToString());
	return FGameplayAttribute();
}

FGameplayTag URsAttributeSetBase::AttributeToTag(const FGameplayAttribute& Attribute)
{
	if (const FGameplayTag* Found = StatTagMap.Find(Attribute))
	{
		return *Found;
	}
	return FGameplayTag::EmptyTag;
}

const TMap<FGameplayTag, FGameplayAttribute>& URsAttributeSetBase::GetStatMap()
{
	return TagStatMap;
}

const TMap<FGameplayTag, FGameplayAttribute>& URsAttributeSetBase::GetCoefficientMap()
{
	return TagCoefficientMap;
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
	TagStatMap.Add(Tag, Attribute);
	StatTagMap.Add(Attribute, Tag);
}

void URsAttributeSetBase::RegisterTagToCoefficient(const FGameplayTag& Tag, FGameplayAttribute Attribute)
{
	TagCoefficientMap.Add(Tag, Attribute);
}
