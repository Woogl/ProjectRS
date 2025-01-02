// Copyright 2024 Team BH.


#include "RsAIControllerBase.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

ARsAIControllerBase::ARsAIControllerBase()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

ETeamAttitude::Type ARsAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const IGenericTeamAgentInterface* TeamInterface = Cast<IGenericTeamAgentInterface>(&Other))
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

	return ETeamAttitude::Neutral;
}

void ARsAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::HandlePlayerControllingPawnChanged);
		
		if (BehaviorTree)
		{
			GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerControllingPawn"), PlayerController->GetPawn());
		}
	}
}

void ARsAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		GetBlackboardComponent()->SetValueAsObject(TEXT("SelfActor"), InPawn);
	}
}

void ARsAIControllerBase::HandlePlayerControllingPawnChanged(APawn* InOldPawn, APawn* InNewPawn)
{
	if (BehaviorTree)
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerControllingPawn"), InNewPawn);
	}
}
