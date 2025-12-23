// Copyright 2025 Team BH.


#include "RsCondition.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

bool URsCondition_StatQuery::IsSatisfied(UObject* ContextObject) const
{
	if (const AActor* Actor = Cast<AActor>(ContextObject))
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor))
		{
			return URsStatQueryLibrary::MatchesStatQuery(ASC, Query);
		}
	}
	return false;
}
