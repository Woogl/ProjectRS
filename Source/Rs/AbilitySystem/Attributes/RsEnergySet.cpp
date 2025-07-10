// Copyright 2024 Team BH.


#include "RsEnergySet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

URsEnergySet::URsEnergySet()
{
	CurrentEnergy = 0.f;
	MaxEnergy = 0.f;
	CurrentSpecial = 0.f;
	MaxSpecial = 0.f;
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
	DOREPLIFETIME_WITH_PARAMS_FAST(URsEnergySet, CurrentSpecial, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsEnergySet, MaxSpecial, Params);
}

void URsEnergySet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxEnergyAttribute())
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
	else if (Data.EvaluatedData.Attribute == GetCurrentSpecialAttribute())
	{
		SetCurrentSpecial(FMath::Clamp(GetCurrentSpecial(), 0.f, GetMaxSpecial()));
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

void URsEnergySet::OnRep_CurrentSpecial(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsEnergySet, CurrentSpecial, OldValue);
}

void URsEnergySet::OnRep_MaxSpecial(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsEnergySet, MaxSpecial, OldValue);
}
