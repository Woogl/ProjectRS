// Copyright 2024 Team BH.


#include "RsAttributeSetBase.h"

#include "RsAttackSet.h"
#include "RsDefenseSet.h"
#include "RsEnergySet.h"
#include "RsHealthSet.h"
#include "RsStaggerSet.h"
#include "RsSpeedSet.h"

float FRsAttributeMetaData::GetValue(const FGameplayAttribute& Attribute) const
{
	if (Attribute == URsHealthSet::GetMaxHealthAttribute())
	{
		return MaxHealth;
	}
	if (Attribute == URsHealthSet::GetCurrentHealthAttribute())
	{
		return CurrentHealth;
	}
	if (Attribute == URsHealthSet::GetBarrierAttribute())
	{
		return Barrier;
	}
	if (Attribute == URsStaggerSet::GetMaxStaggerAttribute())
	{
		return MaxStagger;
	}
	if (Attribute == URsStaggerSet::GetCurrentStaggerAttribute())
	{
		return CurrentStagger;
	}
	if (Attribute == URsStaggerSet::GetStaggerDecayAttribute())
	{
		return StaggerDecay;
	}
	if (Attribute == URsEnergySet::GetMaxManaAttribute())
	{
		return MaxMana;
	}
	if (Attribute == URsEnergySet::GetCurrentManaAttribute())
	{
		return CurrentMana;
	}
	if (Attribute == URsEnergySet::GetMaxEnergyAttribute())
	{
		return MaxEnergy;
	}
	if (Attribute == URsEnergySet::GetCurrentEnergyAttribute())
	{
		return CurrentEnergy;
	}
	if (Attribute == URsAttackSet::GetAttackAttribute())
	{
		return Attack;
	}
	if (Attribute == URsAttackSet::GetImpactAttribute())
	{
		return Impact;
	}
	if (Attribute == URsDefenseSet::GetDefenseAttribute())
	{
		return Defense;
	}
	if (Attribute == URsAttackSet::GetCriticalRateAttribute())
	{
		return CriticalRate;
	}
	if (Attribute == URsAttackSet::GetCriticalDamageAttribute())
	{
		return CriticalDamage;
	}
	if (Attribute == URsDefenseSet::GetInvincibleTierAttribute())
	{
		return Invincible;
	}
	if (Attribute == URsDefenseSet::GetSuperArmorTierAttribute())
	{
		return SuperArmor;
	}
	if (Attribute == URsSpeedSet::GetMoveSpeedMultiplierAttribute())
	{
		return MoveSpeed;
	}
	ensureMsgf(false, TEXT("Unknown attribute: %s"), *Attribute.AttributeName);
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
