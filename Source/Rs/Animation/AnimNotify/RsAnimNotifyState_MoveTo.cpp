// Copyright 2025 Team BH.


#include "RsAnimNotifyState_MoveTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "Rs/Player/RsPlayerController.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

void URsAnimNotifyState_MoveTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return;
	}
	
	if (PositionMode == ERsMoveToPosition::TargetLocalPosition)
	{
		if (AActor* MoveTarget = FindMoveTarget(Owner))
		{
			TargetLocation = MoveTarget->GetActorLocation() + MoveTarget->GetActorTransform().TransformVector(Position);
		}
	}
	else if (PositionMode == ERsMoveToPosition::SourceLocalPosition)
	{
		TargetLocation = Owner->GetActorLocation() + Owner->GetActorTransform().TransformVector(Position);
	}
	else if (PositionMode == ERsMoveToPosition::WorldPosition)
	{
		TargetLocation = Position;
	}
}

void URsAnimNotifyState_MoveTo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return;
	}
	
	float Progress = FMath::Clamp(EventReference.GetCurrentAnimationTime() / EventReference.GetNotify()->Duration, 0.f, 1.f);
	FVector NewLocation = FMath::Lerp(Owner->GetActorLocation(), TargetLocation, Progress);
	Owner->SetActorLocation(NewLocation, true);
}

AActor* URsAnimNotifyState_MoveTo::FindMoveTarget(AActor* Owner) const
{
	if (!Owner)
	{
		return nullptr;
	}

	AActor* TeleportTarget = nullptr;
	if (ACharacter* Character = Cast<ACharacter>(Owner))
	{
		if (AController* Controller = Character->GetController())
		{
			// PlayerCharacter: LockOnComponent
			if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(Controller))
			{
				if (URsLockOnComponent* LockOnComponent = RsPlayerController->GetLockOnComponent())
				{
					TeleportTarget = LockOnComponent->GetLockOnTarget();
				}
			}
			// EnemyCharacter: Blackboard
			else if (AAIController* AIController = Cast<AAIController>(Controller))
			{
				if (UBlackboardComponent* BBComponent = AIController->GetBlackboardComponent())
				{
					if (UObject* BBObject = BBComponent->GetValueAsObject(TEXT("TargetActor")))
					{
						TeleportTarget = Cast<AActor>(BBObject);
					}
				}
			}
		}
	}

	// Fallback: Targeting
	if (bFallbackToTargeting && !TeleportTarget)
	{
		TArray<AActor*> OutActors;
		if (URsTargetingLibrary::PerformTargeting(Owner, Owner->GetActorTransform(), FallbackCollision, FallbackFilter, FallbackSorter, OutActors))
		{
			TeleportTarget = OutActors[0];
		}
	}

	return TeleportTarget;
}
