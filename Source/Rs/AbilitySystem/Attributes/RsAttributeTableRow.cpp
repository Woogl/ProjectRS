﻿// Copyright 2025 Team BH.


#include "RsAttributeTableRow.h"

#include "RsAttackSet.h"
#include "RsDefenseSet.h"
#include "RsEnergySet.h"
#include "RsHealthSet.h"
#include "RsSpeedSet.h"
#include "RsStaggerSet.h"

float FRsAttributeTableRow::GetBaseValue(const FGameplayAttribute& Attribute) const
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
