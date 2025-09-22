// Copyright 2024 Team BH.


#include "RsEnergySet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

URsEnergySet::URsEnergySet()
{
	CurrentEnergy = 0.f;
	MaxEnergy = 1.f;
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
	DOREPLIFETIME_WITH_PARAMS_FAST(URsEnergySet, CurrentEnergy, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsEnergySet, MaxEnergy, Params);

	// Only Owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(URsEnergySet, CurrentMana, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsEnergySet, MaxMana, Params);
}

void URsEnergySet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxEnergyAttribute())
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
	
	if (Data.EvaluatedData.Attribute == GetCurrentEnergyAttribute())
	{
		SetCurrentEnergy(FMath::Clamp(GetCurrentEnergy(), 0.f, GetMaxEnergy()));
	}
	else if (Data.EvaluatedData.Attribute == GetCurrentManaAttribute())
	{
		SetCurrentMana(FMath::Clamp(GetCurrentMana(), 0.f, GetMaxMana()));
	}
}

void URsEnergySet::OnRep_CurrentEnergy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsEnergySet, CurrentEnergy, OldValue);
}

void URsEnergySet::OnRep_MaxEnergy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsEnergySet, MaxEnergy, OldValue);
}

void URsEnergySet::OnRep_CurrentMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsEnergySet, CurrentMana, OldValue);
}

void URsEnergySet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsEnergySet, MaxMana, OldValue);
}
