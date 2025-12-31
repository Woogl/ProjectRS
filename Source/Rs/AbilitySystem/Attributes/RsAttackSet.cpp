// Copyright 2024 Team BH.


#include "RsAttackSet.h"

#include "Net/UnrealNetwork.h"
#include "Rs/RsGameplayTags.h"

URsAttackSet::URsAttackSet()
	: Attack(0.f)
	, Impact(0.f)
	, CriticalRate(0.f)
	, CriticalDamage(0.f)
{
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		RegisterTagToStat(RsGameplayTags::STAT_ATK, GetAttackAttribute());
		RegisterTagToStat(RsGameplayTags::STAT_IMP, GetImpactAttribute());
		RegisterTagToStat(RsGameplayTags::STAT_CRTrate, GetCriticalRateAttribute());
		RegisterTagToStat(RsGameplayTags::STAT_CRTdmg, GetCriticalDamageAttribute());
	
		RegisterTagToCoefficient(RsGameplayTags::COEFFICIENT_ATK_SOURCE, GetAttackAttribute());
		RegisterTagToCoefficient(RsGameplayTags::COEFFICIENT_ATK_TARGET, GetAttackAttribute());
		RegisterTagToCoefficient(RsGameplayTags::COEFFICIENT_IMP_SOURCE, GetImpactAttribute());
		RegisterTagToCoefficient(RsGameplayTags::COEFFICIENT_IMP_TARGET, GetImpactAttribute());
	}
}

void URsAttackSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(URsAttackSet, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URsAttackSet, Impact, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URsAttackSet, CriticalRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URsAttackSet, CriticalDamage, COND_None, REPNOTIFY_Always);
}

void URsAttackSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsAttackSet, Attack, OldValue);
}

void URsAttackSet::OnRep_Impact(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsAttackSet, Impact, OldValue);
}

void URsAttackSet::OnRep_CriticalRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsAttackSet, CriticalRate, OldValue);
}

void URsAttackSet::OnRep_CriticalDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsAttackSet, CriticalDamage, OldValue);
}
