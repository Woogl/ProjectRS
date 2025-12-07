// Copyright 2024 Team BH.


#include "RsSpeedSet.h"

#include "Net/UnrealNetwork.h"
#include "Rs/RsGameplayTags.h"

URsSpeedSet::URsSpeedSet()
	: MoveSpeed(1.f)
	, ActionSpeed(1.f)
{
	RegisterTagToStat(RsGameplayTags::STAT_MOV, GetMoveSpeedAttribute());
	RegisterTagToStat(RsGameplayTags::STAT_ATS, GetActionSpeedAttribute());
}

void URsSpeedSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicated to all
	DOREPLIFETIME_CONDITION_NOTIFY(URsSpeedSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URsSpeedSet, ActionSpeed, COND_None, REPNOTIFY_Always);
}

void URsSpeedSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetMoveSpeedAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
	else if (Attribute == GetActionSpeedAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void URsSpeedSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsSpeedSet, MoveSpeed, OldValue);
}

void URsSpeedSet::OnRep_ActionSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsSpeedSet, ActionSpeed, OldValue);
}
