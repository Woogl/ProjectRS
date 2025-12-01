// Copyright 2024 Team BH.


#include "RsStaggerSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

URsStaggerSet::URsStaggerSet()
{
	MaxStagger = 1.f;
	CurrentStagger = 0.f;
	StaggerDecay = 0.f;

	StaggerDamageCueTag = FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Damage.Stagger"));
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
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStagger());
	}
}

void URsStaggerSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetMaxStaggerAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentStaggerAttribute(), OldValue, NewValue);
	}
}

void URsStaggerSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetFinalDamageAttribute())
	{
		// Store a local copy of the amount of Stagger Gain done and clear the Stagger Gain attribute.
		const float LocalFinalDamage = GetFinalDamage();
		SetBaseDamage(0.f);
		SetFinalDamage(0.f);
		if (LocalFinalDamage > 0.f)
		{
			// Apply the Stagger change and then clamp it.
			const float NewStagger = GetCurrentStagger() + LocalFinalDamage;
			SetCurrentStagger(FMath::Clamp(NewStagger, 0.f, GetMaxStagger()));
		}
		
		FGameplayCueParameters GameplayCueParams;
		GameplayCueParams.EffectContext = Data.EffectSpec.GetEffectContext();
		GameplayCueParams.RawMagnitude = LocalFinalDamage;
		GetOwningAbilitySystemComponent()->ExecuteGameplayCue(StaggerDamageCueTag, GameplayCueParams);
	}
	
	if (Data.EvaluatedData.Attribute == GetCurrentStaggerAttribute())
	{
		SetCurrentStagger(FMath::Clamp(GetCurrentStagger(), 0.f, GetMaxStagger()));
	}

	else if (Data.EvaluatedData.Attribute == GetStaggerDecayAttribute())
	{
		SetStaggerDecay(FMath::Clamp(GetStaggerDecay(), 0.f, GetMaxStagger()));
	}
}

void URsStaggerSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Replicated to all
	DOREPLIFETIME_CONDITION_NOTIFY(URsStaggerSet, CurrentStagger, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URsStaggerSet, MaxStagger, COND_None, REPNOTIFY_Always);

	// Owner Only
	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(URsStaggerSet, StaggerDecay, Params);
}

void URsStaggerSet::OnRep_CurrentStagger(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsStaggerSet, CurrentStagger, OldValue);
}

void URsStaggerSet::OnRep_MaxStagger(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsStaggerSet, MaxStagger, OldValue);
}

void URsStaggerSet::OnRep_StaggerDecay(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsStaggerSet, StaggerDecay, OldValue);
}