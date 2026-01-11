// Copyright 2024 Team BH.


#include "RsDefenseSet.h"

#include "Net/UnrealNetwork.h"
#include "Rs/RsGameplayTags.h"

URsDefenseSet::URsDefenseSet()
	: Defense(0.f)
	, Resistance(0.f)
{
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		RegisterTagToStat(RsGameplayTags::STAT_DEF, GetDefenseAttribute());
	
		RegisterTagToCoefficient(RsGameplayTags::COEFFICIENT_DEF_SOURCE, GetDefenseAttribute());
		RegisterTagToCoefficient(RsGameplayTags::COEFFICIENT_DEF_TARGET, GetDefenseAttribute());
	}
}

void URsDefenseSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(URsDefenseSet, Defense, COND_None, REPNOTIFY_Always);
}

void URsDefenseSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void URsDefenseSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsDefenseSet, Defense, OldValue);
}

void URsDefenseSet::OnRep_Resistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsDefenseSet, Resistance, OldValue);
}
