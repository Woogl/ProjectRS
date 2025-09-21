// Copyright 2024 Team BH.


#include "RsAttributeSetBase.h"

#include "RsAttackSet.h"
#include "RsDefenseSet.h"
#include "RsHealthSet.h"
#include "RsStaggerSet.h"
#include "Rs/RsLogChannels.h"

float FRsAttributeMetaData::GetValue(const FGameplayAttribute& Attribute) const
{
	if (Attribute == URsHealthSet::GetMaxHealthAttribute())
	{
		return HPmax;
	}
	if (Attribute == URsHealthSet::GetCurrentHealthAttribute())
	{
		return HPcur;
	}
	if (Attribute == URsStaggerSet::GetMaxStaggerAttribute())
	{
		return SPmax;
	}
	if (Attribute == URsStaggerSet::GetCurrentStaggerAttribute())
	{
		return SPcur;
	}
	if (Attribute == URsAttackSet::GetAttackAttribute())
	{
		return ATK;
	}
	if (Attribute == URsAttackSet::GetImpactAttribute())
	{
		return IMP;
	}
	if (Attribute == URsDefenseSet::GetDefenseAttribute())
	{
		return DEF;
	}
	if (Attribute == URsAttackSet::GetCriticalRateAttribute())
	{
		return CRITrate;
	}
	if (Attribute == URsAttackSet::GetCriticalDmgBonusAttribute())
	{
		return CRITdmg;
	}
	UE_LOG(RsLog, Warning, TEXT("Cannot find %s attribute in table!"), *Attribute.AttributeName);
	return 0.f;
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
