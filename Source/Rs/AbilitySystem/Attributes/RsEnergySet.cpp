// Copyright 2024 Team BH.


#include "RsEnergySet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

URsEnergySet::URsEnergySet()
{
	CurrentUltimate = 0.f;
	MaxUltimate = 1.f;
	CurrentMana = 0.f;
	MaxMana = 1.f;
}

void URsEnergySet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	
	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(URsEnergySet, CurrentUltimate, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsEnergySet, MaxUltimate, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsEnergySet, CurrentMana, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsEnergySet, MaxMana, Params);
}

void URsEnergySet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxUltimateAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}

	if (Attribute == GetMaxManaAttribute())
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
