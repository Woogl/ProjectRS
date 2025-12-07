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
	RegisterTagToStat(RsGameplayTags::STAT_ATK, GetAttackAttribute());
	RegisterTagToStat(RsGameplayTags::STAT_IMP, GetImpactAttribute());
	RegisterTagToStat(RsGameplayTags::STAT_CRTrate, GetCriticalRateAttribute());
	RegisterTagToStat(RsGameplayTags::STAT_CRTdmg, GetCriticalRateAttribute());
	
	RegisterTagToCoefficient(RsGameplayTags::COEFFICIENT_ATK_SOURCE, GetAttackAttribute());
	RegisterTagToCoefficient(RsGameplayTags::COEFFICIENT_ATK_TARGET, GetAttackAttribute());
	RegisterTagToCoefficient(RsGameplayTags::COEFFICIENT_IMP_SOURCE, GetImpactAttribute());
	RegisterTagToCoefficient(RsGameplayTags::COEFFICIENT_IMP_TARGET, GetImpactAttribute());
}

void URsAttackSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	
	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(URsAttackSet, Attack, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsAttackSet, Impact, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsAttackSet, CriticalRate, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsAttackSet, CriticalDamage, Params);
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
