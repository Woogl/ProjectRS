// Copyright 2024 Team BH.


#include "BTTask_RandomMove.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Tasks/AITask_MoveTo.h"

UBTTask_RandomMove::UBTTask_RandomMove()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_RandomMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (APawn* Pawn = Controller->GetPawn())
	{
		FVector PawnLocation = Pawn->GetActorLocation();
		FVector ForwardVector = Pawn->GetActorForwardVector();

		float MinAngle;
		float MaxAngle;

		switch (Direction)
		{
		case EMoveDirection::Forward:
			MinAngle = -30.f;
			MaxAngle = 30.f;
			break;
		case EMoveDirection::Backward:
			MinAngle = 150.f;
			MaxAngle = 210.f;
			break;
		case EMoveDirection::Leftward:
			MinAngle = 240.f;
			MaxAngle = 300.f;
			break;
		case EMoveDirection::Rightward:
			MinAngle = 60.f;
			MaxAngle = 120.f;
			break;
		default:
			MinAngle = 0.f;
			MaxAngle = 360.f;
			break;
		}
		FVector TargetLocation = GetRandomReachablePointInAngle(PawnLocation, ForwardVector, MinAngle, MaxAngle);
		if (TargetLocation != PawnLocation)
		{
			MyOwnerComp = &OwnerComp;
			FAIMoveRequest MoveReq(TargetLocation);
			Controller->MoveTo(MoveReq);
			Controller->ReceiveMoveCompleted.AddDynamic(this, &UBTTask_RandomMove::OnMoveFinished);
			return EBTNodeResult::InProgress;
		}
	}
	return EBTNodeResult::Failed;
}

FVector UBTTask_RandomMove::GetRandomReachablePointInAngle(const FVector& Origin, const FVector& Forward, float MinAngle, float MaxAngle)
{
	if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		// try 10 times for maximum, relax conditions when fail
		for (int i = 0; i < 10; i++)
		{
			float RandomAngle = FMath::RandRange(MinAngle, MaxAngle);
			FVector RandomDirection = Forward.RotateAngleAxis(RandomAngle, FVector::UpVector);
			float RandomDistance = FMath::RandRange(Distance * 0.8f, Distance * 1.2f);
			FVector RandomLocation = Origin + RandomDirection * RandomDistance;

			FNavLocation TargetLocation;
			if (NavSys->ProjectPointToNavigation(RandomLocation, TargetLocation))
			{
				return RandomLocation;
			}
			MinAngle -= 3.f;
			MaxAngle += 3.f;
			Distance -= 30.f;
		}
	}
	return Origin;
}

void UBTTask_RandomMove::OnMoveFinished(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (MyOwnerComp.IsValid())
	{
		MyOwnerComp->GetAIOwner()->ReceiveMoveCompleted.RemoveDynamic(this, &UBTTask_RandomMove::OnMoveFinished);
		FinishLatentTask(*MyOwnerComp,Result == EPathFollowingResult::Success ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
	}
}
