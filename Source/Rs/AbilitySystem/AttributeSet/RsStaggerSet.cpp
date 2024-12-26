// Copyright 2024 Team BH.


#include "RsStaggerSet.h"

#include "GameplayEffectExtension.h"

URsStaggerSet::URsStaggerSet()
{
	MaximumStagger = 0.f;
	CurrentStagger = 0.f;
	StaggerRegeneration = 0.f;
}

void URsStaggerSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaximumStaggerAttribute())
	{
		AdjustAttributeForMaxChange(CurrentStagger, MaximumStagger, NewValue, GetCurrentStaggerAttribute());
	}
}

void URsStaggerSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetCurrentStaggerAttribute())
	{
		SetCurrentStagger(FMath::Clamp(GetCurrentStagger(), 0.0f, GetMaximumStagger()));
	}

	else if (Data.EvaluatedData.Attribute == GetStaggerRegenerationAttribute())
	{
		SetStaggerRegeneration(FMath::Clamp(GetStaggerRegeneration(), 0.0f, GetMaximumStagger()));
	}
}

void URsStaggerSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void URsStaggerSet::OnRep_CurrentStagger(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsStaggerSet, CurrentStagger, OldValue);
}

void URsStaggerSet::OnRep_MaximumStagger(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsStaggerSet, MaximumStagger, OldValue);
}

void URsStaggerSet::OnRep_StaggerRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsStaggerSet, StaggerRegeneration, OldValue);

}