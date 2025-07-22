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

	Duration = TotalDuration;
	Elapsed = 0.f;

	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return;
	}
	
	if (PositionMode != ERsPositionMode::WorldPosition)
	{
		Target = FindMoveTarget(Owner);
	}

	StartLocation = Owner->GetActorLocation();
	AcceptableRadiusSquared = AcceptableRadius * AcceptableRadius;
}

void URsAnimNotifyState_MoveTo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	Elapsed += FrameDeltaTime;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	bool bValidTarget = Target.IsValid();
	FVector TargetLocation = Owner->GetActorLocation();
	
	if (PositionMode == ERsPositionMode::LocalPosition_Target && bValidTarget)
	{
		TargetLocation = Target->GetActorLocation() + Target->GetActorTransform().TransformVector(Position);
	}
	else if (PositionMode == ERsPositionMode::LocalPosition_Source && bValidTarget)
	{
		TargetLocation = Owner->GetActorLocation() + Owner->GetActorTransform().TransformVector(Position);
	}
	else if (PositionMode == ERsPositionMode::WorldPosition)
	{
		TargetLocation = Position;
	}
	else if (PositionMode == ERsPositionMode::TowardTarget && bValidTarget)
	{
		TargetLocation = Target->GetActorLocation();
	}

	if (MaxMoveDistance > 0.f)
	{
		if (FVector::DistSquared(StartLocation, TargetLocation) > MaxMoveDistance * MaxMoveDistance)
		{
			TargetLocation = StartLocation + (TargetLocation - StartLocation).GetSafeNormal() * MaxMoveDistance;
		}
	}

	if (FVector::DistSquared(Owner->GetActorLocation(), TargetLocation) > AcceptableRadiusSquared)
	{
		FVector NewLocation = FMath::Lerp(Owner->GetActorLocation(), TargetLocation, GetNotifyProgress());
		Owner->SetActorLocation(NewLocation, true);
	}
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

float URsAnimNotifyState_MoveTo::GetNotifyProgress() const
{
	return FMath::Clamp(Elapsed / FMath::Max(Duration, SMALL_NUMBER), 0.f, 1.f);
}
