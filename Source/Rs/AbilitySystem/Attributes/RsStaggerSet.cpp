// Copyright 2024 Team BH.


#include "RsStaggerSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

URsStaggerSet::URsStaggerSet()
{
	MaxStagger = 0.f;
	CurrentStagger = 0.f;
	StaggerRegen = 0.f;
}

void URsStaggerSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxStaggerAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}
	
	else if (Attribute == GetCurrentStaggerAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStagger());
	}
}

void URsStaggerSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetStaggerDamageAttribute())
	{
		// Store a local copy of the amount of Stagger Gain done and clear the Stagger Gain attribute.
		const float LocalDamage = GetStaggerDamage();

		SetStaggerDamage(0.f);
	
		if (LocalDamage > 0.0f)
		{
			// Apply the Stagger change and then clamp it.
			const float NewStagger = GetCurrentStagger() + LocalDamage;
			SetCurrentStagger(FMath::Clamp(NewStagger, 0.0f, GetMaxStagger()));
		}
	}
	
	if (Data.EvaluatedData.Attribute == GetCurrentStaggerAttribute())
	{
		SetCurrentStagger(FMath::Clamp(GetCurrentStagger(), 0.0f, GetMaxStagger()));
	}

	else if (Data.EvaluatedData.Attribute == GetStaggerRegenAttribute())
	{
		SetStaggerRegen(FMath::Clamp(GetStaggerRegen(), 0.0f, GetMaxStagger()));
	}
}

void URsStaggerSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(URsStaggerSet, CurrentStagger, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsStaggerSet, MaxStagger, Params);

	// Owner Only
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(URsStaggerSet, StaggerRegen, Params);
}

void URsStaggerSet::OnRep_CurrentStagger(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsStaggerSet, CurrentStagger, OldValue);
}

void URsStaggerSet::OnRep_MaxStagger(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsStaggerSet, MaxStagger, OldValue);
}

void URsStaggerSet::OnRep_StaggerRegen(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsStaggerSet, StaggerRegen, OldValue);

}