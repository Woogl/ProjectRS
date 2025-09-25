// Copyright 2025 Team BH.


#include "RsCrowdControlEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbilityTypes.h"

void URsCrowdControlEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);
	
	if (!ensureMsgf(ActiveGEContainer.Owner, TEXT("OnGameplayEffectApplied is passed an ActiveGEContainer which lives within an ASC but that ASC was somehow null")))
	{
		return;
	}

	if (EventTag.IsValid())
	{
		FGameplayEventData Payload;
		Payload.EventTag = EventTag;
		Payload.Instigator = GESpec.GetEffectContext().GetInstigator();
		Payload.Target = GESpec.GetEffectContext().GetHitResult()->GetActor();
		Payload.InstigatorTags = GESpec.CapturedSourceTags.GetActorTags();
		Payload.TargetTags = GESpec.CapturedTargetTags.GetActorTags();
		Payload.EventMagnitude = GESpec.GetLevel();
		Payload.ContextHandle = GESpec.GetEffectContext();
			
		ActiveGEContainer.Owner->HandleGameplayEvent(EventTag, &Payload);
	}
}
