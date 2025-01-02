// Copyright 2024 Team BH.


#include "RsTargetingFilterTask_TeamID.h"

#include "GenericTeamAgentInterface.h"
#include "Rs/AI/RsAILibrary.h"

bool URsTargetingFilterTask_TeamID::ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const
{
	uint8 TargetActorTeamID = FGenericTeamId::NoTeam.GetId();
	AActor* TargetActor = TargetData.HitResult.GetActor();
	if (TargetActor)
	{
		TargetActorTeamID = URsAILibrary::GetTeamID(TargetActor);
	}

	uint8 SourceActorTeamID = FGenericTeamId::NoTeam.GetId();
	if (const FTargetingSourceContext* SourceContext = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (SourceContext->SourceActor)
		{
			SourceActorTeamID = URsAILibrary::GetTeamID(SourceContext->SourceActor);
		}
	}
	
	if (TargetActor)
	{
		// if the target is one of these IDs, filter it out
		for (uint8 IDFilter : IgnoredTeamIDs)
		{
			if (IDFilter == TargetActorTeamID)
			{
				return true;
			}
		}

		// if the target is one of these IDs, do not filter it out
		for (uint8 IDFilter : RequiredTeamIDs)
		{
			if (IDFilter == TargetActorTeamID)
			{
				return false;
			}
		}
	}

	if (bIgnoreSourceTeamID && SourceActorTeamID == TargetActorTeamID)
	{
		return true;
	}

	return false;
}
