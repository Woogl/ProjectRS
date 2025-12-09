// Copyright 2024 Team BH.


#include "RsDefenseSet.h"

#include "Net/UnrealNetwork.h"
#include "Rs/RsGameplayTags.h"

URsDefenseSet::URsDefenseSet()
	: Defense(0.f)
{
	RegisterTagToStat(RsGameplayTags::STAT_DEF, GetDefenseAttribute());
	RegisterTagToStat(RsGameplayTags::STAT_INV, GetInvincibleAttribute());
	RegisterTagToStat(RsGameplayTags::STAT_SUA, GetSuperArmorAttribute());
	
	RegisterTagToCoefficient(RsGameplayTags::COEFFICIENT_DEF_SOURCE, GetDefenseAttribute());
	RegisterTagToCoefficient(RsGameplayTags::COEFFICIENT_DEF_TARGET, GetDefenseAttribute());
}

void URsDefenseSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(URsDefenseSet, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URsDefenseSet, Invincible, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URsDefenseSet, SuperArmor, COND_None, REPNOTIFY_Always);
}

void URsDefenseSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetInvincibleAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
	
	else if (Attribute == GetSuperArmorAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void URsDefenseSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsDefenseSet, Defense, OldValue);
}

void URsDefenseSet::OnRep_Invincible(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsDefenseSet, Invincible, OldValue);
}

void URsDefenseSet::OnRep_SuperArmor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsDefenseSet, SuperArmor, OldValue);
}
