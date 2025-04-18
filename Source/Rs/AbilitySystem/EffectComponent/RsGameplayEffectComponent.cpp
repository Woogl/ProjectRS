// Copyright 2024 Team BH.


#include "RsGameplayEffectComponent.h"

#include "GameplayEffect.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"

bool URsGameplayEffectComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer,FActiveGameplayEffect& ActiveGE) const
{
	if (const URsHealthSet* TargetSet = ActiveGEContainer.Owner->GetSet<URsHealthSet>())
	{
		const_cast<URsHealthSet*>(TargetSet)->AddNewShield(ActiveGE);
		ActiveGE.EventSet.OnEffectRemoved.AddUObject(this,&URsGameplayEffectComponent::OnActiveGameplayEffectRemoved,&ActiveGEContainer);
	}
	return Super::OnActiveGameplayEffectAdded(ActiveGEContainer, ActiveGE);
}

void URsGameplayEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer,FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);
}

void URsGameplayEffectComponent::OnActiveGameplayEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo, FActiveGameplayEffectsContainer* ActiveGEContainer) const
{
	if (const URsHealthSet* TargetSet = ActiveGEContainer->Owner->GetSet<URsHealthSet>())
	{
		const_cast<URsHealthSet*>(TargetSet)->RemoveExpiredShield(*RemovalInfo.ActiveEffect);
	}
}
