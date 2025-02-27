// Copyright 2024 Team BH.


#include "RsSendGameplayEventGEComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Misc/DataValidation.h"

#define LOCTEXT_NAMESPACE "RsSendGameplayEventGEComponent"

void URsSendGameplayEventGEComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec,FPredictionKey& PredictionKey) const
{
	if (!ensureMsgf(ActiveGEContainer.Owner, TEXT("OnGameplayEffectApplied is passed an ActiveGEContainer which lives within an ASC but that ASC was somehow null")))
	{
		return;
	}

	if (!EventTags.IsEmpty())
	{
		for (FGameplayTag EventTag : EventTags)
		{
			FGameplayEventData Payload;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ActiveGEContainer.Owner->GetOwnerActor(),EventTag,Payload);
		}
	}
}

#if WITH_EDITOR
EDataValidationResult URsSendGameplayEventGEComponent::IsDataValid(class FDataValidationContext& Context) const
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
