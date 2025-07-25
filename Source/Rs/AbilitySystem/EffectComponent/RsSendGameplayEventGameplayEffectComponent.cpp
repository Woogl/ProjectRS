// Copyright 2024 Team BH.


#include "RsSendGameplayEventGameplayEffectComponent.h"
#include "AbilitySystemComponent.h"
#include "Misc/DataValidation.h"

#define LOCTEXT_NAMESPACE "RsSendGameplayEventGEComponent"

void URsSendGameplayEventGameplayEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	if (!ensureMsgf(ActiveGEContainer.Owner, TEXT("OnGameplayEffectApplied is passed an ActiveGEContainer which lives within an ASC but that ASC was somehow null")))
	{
		return;
	}

	if (!EventTags.IsEmpty())
	{
		for (const FGameplayTag& EventTag : EventTags)
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
}

#if WITH_EDITOR
EDataValidationResult URsSendGameplayEventGameplayEffectComponent::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if (EventTags.IsEmpty())
	{
		Context.AddWarning(LOCTEXT("EventWithNoParams", "Sending Gameplay Event to Target with no params. It won't operate any GA."));
		Result = EDataValidationResult::Invalid;
	}
	return Result;
}
#endif

#undef LOCTEXT_NAMESPACE
