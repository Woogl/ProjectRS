// Copyright 2025 Team BH.


#include "RsHitStopEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_PauseMontage.h"

void URsHitStopEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);

	UAbilitySystemComponent* SourceASC = GESpec.GetEffectContext().GetInstigatorAbilitySystemComponent();
	if (SourceHitStopTime > 0.f && SourceASC && SourceASC->GetAnimatingAbility())
	{
		if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(SourceASC->GetAnimatingAbility(), SourceHitStopTime))
		{
			PauseMontageTask->ReadyForActivation();
		}
	}

	UAbilitySystemComponent* TargetASC = ActiveGEContainer.Owner;
	if (TargetHitStopTime > 0.f && TargetASC && TargetASC->GetAnimatingAbility())
	{
		if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(TargetASC->GetAnimatingAbility(), TargetHitStopTime, TargetHitStopTime))
		{
			PauseMontageTask->ReadyForActivation();
		}
	}
}
