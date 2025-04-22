// Copyright 2024 Team BH.


#include "RsMovementSet.h"

#include "Net/UnrealNetwork.h"

URsMovementSet::URsMovementSet()
{
	MoveSpeedMultiplier = 1.0f;
}

void URsMovementSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(URsMovementSet, MoveSpeedMultiplier, Params);
}

void URsMovementSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	NewValue = FMath::Max(NewValue, 0.f);
}

void URsMovementSet::OnRep_MoveSpeedMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsMovementSet, MoveSpeedMultiplier, OldValue);
}
