// Copyright 2024 Team BH.


#include "RsAttackSet.h"

#include "Net/UnrealNetwork.h"

URsAttackSet::URsAttackSet()
{
	Attack = 0.f;
	Impact = 0.f;
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
}

void URsAttackSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsAttackSet, Attack, OldValue);
}

void URsAttackSet::OnRep_Impact(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsAttackSet, Impact, OldValue);
}
