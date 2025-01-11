// Copyright 2024 Team BH.


#include "RsDefenseSet.h"

#include "Net/UnrealNetwork.h"

URsDefenseSet::URsDefenseSet()
{
	Defense = 0.f;
}

void URsDefenseSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	
	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(URsDefenseSet, Defense, Params);
}

void URsDefenseSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsDefenseSet, Defense, OldValue);

}
