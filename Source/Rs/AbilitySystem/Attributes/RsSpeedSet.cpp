// Copyright 2024 Team BH.


#include "RsSpeedSet.h"

#include "Net/UnrealNetwork.h"

URsSpeedSet::URsSpeedSet()
{
	Movement = 1.0f;
}

void URsSpeedSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(URsSpeedSet, Movement, Params);
}

void URsSpeedSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	NewValue = FMath::Max(NewValue, 0.f);
}

void URsSpeedSet::OnRep_Movement(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsSpeedSet, Movement, OldValue);
}
