// Copyright 2024 Team BH.


#include "DefenseSet.h"

#include "Net/UnrealNetwork.h"

UDefenseSet::UDefenseSet()
{
	Defense = 0.f;
	Will = 0.f;
}

void UDefenseSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	
	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UDefenseSet, Defense, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UDefenseSet, Will, Params);
}

void UDefenseSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDefenseSet, Defense, OldValue);

}

void UDefenseSet::OnRep_Will(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDefenseSet, Will, OldValue);
}
