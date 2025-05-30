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
	DOREPLIFETIME_WITH_PARAMS_FAST(URsDefenseSet, InvincibleTier, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsDefenseSet, SuperArmorTier, Params);
}

void URsDefenseSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetInvincibleTierAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
	
	else if (Attribute == GetSuperArmorTierAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void URsDefenseSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsDefenseSet, Defense, OldValue);
}

void URsDefenseSet::OnRep_InvincibleTier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsDefenseSet, InvincibleTier, OldValue);
}

void URsDefenseSet::OnRep_SuperArmorTier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsDefenseSet, SuperArmorTier, OldValue);
}
