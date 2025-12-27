// Copyright 2025 Team BH.


#include "RsCondition.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "Rs/RsLogChannels.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

bool URsCondition_StatQuery::IsSatisfied(UObject* ContextObject) const
{
	if (const AActor* Actor = Cast<AActor>(ContextObject))
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor))
		{
			return URsStatQueryLibrary::MatchesStatQuery(ASC, StatQuery);
		}
	}
	return false;
}

bool URsCondition_TagRequirements::IsSatisfied(UObject* ContextObject) const
{
	if (const IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(ContextObject))
	{
		FGameplayTagContainer OutTags;
		TagInterface->GetOwnedGameplayTags(OutTags);
		return TagRequirements.RequirementsMet(OutTags);
	}
	return false;
}

bool URsCondition_Targeting::IsSatisfied(UObject* ContextObject) const
{
	if (const ACharacter* Character = Cast<ACharacter>(ContextObject))
	{
		const USkeletalMeshComponent* MeshComp = Character ? Character->GetMesh() : nullptr;
		return URsTargetingLibrary::PerformTargetingInMeshSpace(MeshComp, TargetingParams, OutActors);
	}
	return false;
}

bool URsCondition_AND::IsSatisfied(UObject* ContextObject) const
{
	for (int32 i = 0; i < Conditions.Num(); i++)
	{
		if (!Conditions[i])
		{
			UE_LOG(LogRs, Warning, TEXT("RsCondition is empty."));
			continue;
		}
		if (!Conditions[i]->IsSatisfied(ContextObject))
		{
			return false;
		}
	}
	return true;
}

bool URsCondition_OR::IsSatisfied(UObject* ContextObject) const
{
	for (int32 i = 0; i < Conditions.Num(); i++)
	{
		if (!Conditions[i])
		{
			UE_LOG(LogRs, Warning, TEXT("RsCondition is empty."));
			continue;
		}
		if (Conditions[i]->IsSatisfied(ContextObject))
		{
			return true;
		}
	}
	return false;
}
