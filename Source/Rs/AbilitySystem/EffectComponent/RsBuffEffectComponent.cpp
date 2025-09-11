// Copyright 2025 Team BH.


#include "RsBuffEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Rs/Battle/RsBattleSettings.h"

void URsBuffEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	if (!ensureMsgf(ActiveGEContainer.Owner, TEXT("OnGameplayEffectApplied is passed an ActiveGEContainer which lives within an ASC but that ASC was somehow null")))
	{
		return;
	}

	const float GELevel = GESpec.GetLevel();
	const FGameplayEffectContextHandle& GEContextHandle = GESpec.GetEffectContext();

	FConditionalGameplayEffect ConditionalEffect;
	ConditionalEffect.EffectClass = URsBattleSettings::Get().BuffEffects.FindRef(Stat);
	
	/** Child effect that need to be applied to the target if parent effect is successful */
	FGameplayEffectSpecHandle SpecHandle;
	if (ConditionalEffect.CanApply(GESpec.CapturedSourceTags.GetActorTags(), GELevel))
	{
		SpecHandle = ConditionalEffect.CreateSpec(GEContextHandle, GELevel);
	}

	UAbilitySystemComponent* AppliedToASC = nullptr;
	if (Target == ERsEffectTarget::Source)
	{
		AppliedToASC = GESpec.GetEffectContext().GetInstigatorAbilitySystemComponent();
	}
	else if (Target == ERsEffectTarget::Target)
	{
		AppliedToASC = ActiveGEContainer.Owner;
	}

	if (AppliedToASC)
	{
		AppliedToASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get(), PredictionKey);
	}
}
