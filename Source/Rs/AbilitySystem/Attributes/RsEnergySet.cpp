// Copyright 2024 Team BH.


#include "RsEnergySet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Rs/RsGameplayTags.h"

URsEnergySet::URsEnergySet()
	: CurrentUltimate(0.f)
	, MaxUltimate(1.f)
	, CurrentMana(0.f)
	, MaxMana(1.f)
{
	RegisterTagToStat(RsGameplayTags::STAT_UPcur, GetCurrentUltimateAttribute());
	RegisterTagToStat(RsGameplayTags::STAT_UPmax, GetMaxUltimateAttribute());
	RegisterTagToStat(RsGameplayTags::STAT_MPcur, GetCurrentManaAttribute());
	RegisterTagToStat(RsGameplayTags::STAT_MPmax, GetMaxManaAttribute());
}

void URsEnergySet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(URsEnergySet, CurrentUltimate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URsEnergySet, MaxUltimate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URsEnergySet, CurrentMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URsEnergySet, MaxMana, COND_None, REPNOTIFY_Always);
}

void URsEnergySet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxUltimateAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}
}

void URsEnergySet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetCurrentUltimateAttribute())
	{
		SetCurrentUltimate(FMath::Clamp(GetCurrentUltimate(), 0.f, GetMaxUltimate()));
	}
	else if (Data.EvaluatedData.Attribute == GetCurrentManaAttribute())
	{
		SetCurrentMana(FMath::Clamp(GetCurrentMana(), 0.f, GetMaxMana()));
	}
}

void URsEnergySet::OnRep_CurrentUltimate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsEnergySet, CurrentUltimate, OldValue);
}

void URsEnergySet::OnRep_MaxUltimate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsEnergySet, MaxUltimate, OldValue);
}

void URsEnergySet::OnRep_CurrentMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsEnergySet, CurrentMana, OldValue);
}

void URsEnergySet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsEnergySet, MaxMana, OldValue);
}
