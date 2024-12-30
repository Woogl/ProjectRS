// Copyright 2024 Team BH.


#include "RsEnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

ARsEnemyAIController::ARsEnemyAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

ETeamAttitude::Type ARsEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamInterface = Cast<IGenericTeamAgentInterface>(OtherPawn))
		{
			if (TeamInterface->GetGenericTeamId().GetId() != GetGenericTeamId().GetId())
			{
				return ETeamAttitude::Hostile;
			}
			else
			{
				return ETeamAttitude::Friendly;
			}
		}
	}

	return ETeamAttitude::Neutral;
}

void ARsEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		GetBlackboardComponent()->SetValueAsObject(TEXT("SelfActor"), InPawn);
		GetBlackboardComponent()->SetValueAsVector(TEXT("InitialLocation"), InPawn->GetActorLocation());
	}
}
