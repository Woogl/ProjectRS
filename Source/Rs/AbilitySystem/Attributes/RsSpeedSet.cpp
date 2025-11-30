// Copyright 2024 Team BH.


#include "RsSpeedSet.h"

#include "Net/UnrealNetwork.h"

URsSpeedSet::URsSpeedSet()
{
	Movement = 1.f;
	ActionSpeed = 1.f;
}

void URsSpeedSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(URsSpeedSet, Movement, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsSpeedSet, ActionSpeed, Params);
}

void URsSpeedSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetMovementAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
	else if (Attribute == GetActionSpeedAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void URsSpeedSet::OnRep_Movement(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsSpeedSet, Movement, OldValue);
}

void URsSpeedSet::OnRep_ActionSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsSpeedSet, ActionSpeed, OldValue);
}
