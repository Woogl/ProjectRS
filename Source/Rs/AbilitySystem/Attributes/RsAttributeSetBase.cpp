// Copyright 2024 Team BH.


#include "RsAttributeSetBase.h"

#include "AbilitySystemBlueprintLibrary.h"

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

void URsAttributeSetBase::SendEventIfAttributeOverMax(const FGameplayTag& EventTag, const FGameplayAttributeData& MaxAttribute, const FGameplayAttributeData& CurrentAttribute) const
{
	if (MaxAttribute.GetCurrentValue() <= CurrentAttribute.GetCurrentValue())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), EventTag, FGameplayEventData());
	}
}

void URsAttributeSetBase::SendEventIfAttributeBelowZero(const FGameplayTag& EventTag, float OldValue, float NewValue) const
{
	if (NewValue <= 0.f && OldValue > 0.f)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), EventTag, FGameplayEventData());
	}
}
