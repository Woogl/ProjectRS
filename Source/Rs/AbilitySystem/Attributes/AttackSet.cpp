// Copyright 2024 Team BH.


#include "AttackSet.h"

#include "Net/UnrealNetwork.h"

UAttackSet::UAttackSet()
{
	Attack = 0.f;
	Impact = 0.f;
}

void UAttackSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	
	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttackSet, Attack, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttackSet, Impact, Params);
}

void UAttackSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttackSet, Attack, OldValue);
}

void UAttackSet::OnRep_Impact(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttackSet, Impact, OldValue);
}
