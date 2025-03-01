// Copyright 2024 Team BH.


#include "RsAILibrary.h"

#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

uint8 URsAILibrary::GetTeamID(AActor* Actor)
{
	if (IGenericTeamAgentInterface* TeamInterface = Cast<IGenericTeamAgentInterface>(Actor))
	{
		return TeamInterface->GetGenericTeamId().GetId();
	}
	return FGenericTeamId::NoTeam.GetId();
}

UObject* URsAILibrary::GetBlackboardValueAsObject(AActor* Actor, FName KeyName)
{
	if (UBlackboardComponent* BlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(Actor))
	{
		return BlackBoard->GetValueAsObject(KeyName);
	}
	return nullptr;
}
