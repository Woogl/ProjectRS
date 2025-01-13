// Copyright 2024 Team BH.


#include "RsEnergySet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

URsEnergySet::URsEnergySet()
{
	CurrentEnergy = 0.f;
	MaxEnergy = 0.f;
	EnergyRegen = 0.f;
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
	DOREPLIFETIME_WITH_PARAMS_FAST(URsEnergySet, EnergyRegen, Params);
}

void URsEnergySet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxEnergyAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}
	
	else if (Attribute == GetCurrentEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxEnergy());
	}
}

void URsEnergySet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetCurrentEnergyAttribute())
	{
		SetCurrentEnergy(FMath::Clamp(GetCurrentEnergy(), 0.0f, GetMaxEnergy()));
	}

	else if (Data.EvaluatedData.Attribute == GetEnergyRegenAttribute())
	{
		SetEnergyRegen(FMath::Clamp(GetEnergyRegen(), 0.0f, GetMaxEnergy()));
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

void URsEnergySet::OnRep_EnergyRegen(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsEnergySet, EnergyRegen, OldValue);
}
