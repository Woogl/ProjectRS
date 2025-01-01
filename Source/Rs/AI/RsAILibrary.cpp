// Copyright 2024 Team BH.


#include "RsAILibrary.h"

#include "GenericTeamAgentInterface.h"

uint8 URsAILibrary::GetTeamID(AActor* Actor)
{
	if (IGenericTeamAgentInterface* TeamInterface = Cast<IGenericTeamAgentInterface>(Actor))
	{
		return TeamInterface->GetGenericTeamId().GetId();
	}
	return FGenericTeamId::NoTeam.GetId();
}
