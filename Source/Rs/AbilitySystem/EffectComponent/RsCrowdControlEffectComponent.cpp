// Copyright 2025 Team BH.


#include "RsCrowdControlEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "RsCanApplyEffectComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"

URsCrowdControlEffectComponent::URsCrowdControlEffectComponent()
	: Stat(RsGameplayTags::STAT_SUA), Comparision(ERsComparisionOperator::Less), Value(1.f)
{
}

void URsCrowdControlEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);
	
	if (!ensureMsgf(ActiveGEContainer.Owner, TEXT("OnGameplayEffectApplied is passed an ActiveGEContainer which lives within an ASC but that ASC was somehow null")))
	{
		return;
	}

	if (!InternalCanApply(ActiveGEContainer))
	{
		return;
	}

	if (EventTag.IsValid())
	{
		FGameplayEventData Payload;
		Payload.EventTag = EventTag;
		Payload.Instigator = GESpec.GetEffectContext().GetInstigator();
		Payload.Target = ActiveGEContainer.Owner->GetOwner();
		Payload.InstigatorTags = GESpec.CapturedSourceTags.GetActorTags();
		Payload.TargetTags = GESpec.CapturedTargetTags.GetActorTags();
		Payload.ContextHandle = GESpec.GetEffectContext();
			
		ActiveGEContainer.Owner->HandleGameplayEvent(EventTag, &Payload);
	}
}

bool URsCrowdControlEffectComponent::InternalCanApply(const FActiveGameplayEffectsContainer& ActiveGEContainer) const
{
	if (bUseInternalCanApply == false)
	{
		return true;
	}

	bool bResult = false;
	if (Comparision != ERsComparisionOperator::None)
	{
		float StatValue = URsAbilitySystemLibrary::GetNumericAttributeByTag(ActiveGEContainer.Owner, Stat);
		switch (Comparision)
		{
		case ERsComparisionOperator::None:
			bResult = true;
			break;
		case ERsComparisionOperator::Greater:
			bResult = StatValue < Value;
			break;
		case ERsComparisionOperator::Equal:
			bResult = StatValue != Value;
			break;
		case ERsComparisionOperator::Less:
			bResult = StatValue > Value;
			break;
		}
	}

	if (!TagRequirements.IsEmpty())
	{
		FGameplayTagContainer Tags = ActiveGEContainer.Owner->GetOwnedGameplayTags();
		return bResult & !TagRequirements.RequirementsMet(Tags);
	}
	
	return true;
}
