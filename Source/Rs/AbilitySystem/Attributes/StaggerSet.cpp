// Copyright 2024 Team BH.


#include "StaggerSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UStaggerSet::UStaggerSet()
{
	MaxStagger = 0.f;
	CurrentStagger = 0.f;
	StaggerRegen = 0.f;
}

void UStaggerSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxStaggerAttribute())
	{
		AdjustAttributeForMaxChange(CurrentStagger, MaxStagger, NewValue, GetCurrentStaggerAttribute());
	}
}

void UStaggerSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetCurrentStaggerAttribute())
	{
		SetCurrentStagger(FMath::Clamp(GetCurrentStagger(), 0.0f, GetMaxStagger()));
	}

	else if (Data.EvaluatedData.Attribute == GetStaggerRegenAttribute())
	{
		SetStaggerRegen(FMath::Clamp(GetStaggerRegen(), 0.0f, GetMaxStagger()));
	}
}

void UStaggerSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UStaggerSet, CurrentStagger, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UStaggerSet, MaxStagger, Params);

	// Owner Only
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UStaggerSet, StaggerRegen, Params);
}

void UStaggerSet::OnRep_CurrentStagger(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaggerSet, CurrentStagger, OldValue);
}

void UStaggerSet::OnRep_MaxStagger(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaggerSet, MaxStagger, OldValue);
}

void UStaggerSet::OnRep_StaggerRegen(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaggerSet, StaggerRegen, OldValue);

}