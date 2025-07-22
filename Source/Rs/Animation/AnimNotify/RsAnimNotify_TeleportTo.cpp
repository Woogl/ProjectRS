// Copyright 2025 Team BH.


#include "RsAnimNotify_TeleportTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "Rs/Player/RsPlayerController.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotify_TeleportTo::URsAnimNotify_TeleportTo()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotify_TeleportTo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	FVector CurrentLocation = Owner->GetActorLocation();
	FVector NewLocation = Owner->GetActorLocation();
	FRotator NewRotation = Owner->GetActorRotation();
	AActor* TeleportTarget = nullptr;
	
	if (PositionMode == ERsPositionMode::LocalPosition_Target && TeleportTarget)
	{
		NewLocation = TeleportTarget->GetActorLocation() + TeleportTarget->GetActorTransform().TransformVector(Position);
	}
	else if (PositionMode == ERsPositionMode::LocalPosition_Source && TeleportTarget)
	{
		NewLocation = CurrentLocation + Owner->GetActorTransform().TransformVector(Position);
	}
	else if (PositionMode == ERsPositionMode::WorldPosition)
	{
		NewLocation = Position;
	}
	else if (PositionMode == ERsPositionMode::TowardTarget && TeleportTarget)
	{
		NewLocation = TeleportTarget->GetActorLocation();
	}

	if (MaxMoveDistance > 0.f)
	{
		if (FVector::DistSquared(CurrentLocation, NewLocation) > MaxMoveDistance * MaxMoveDistance)
		{
			NewLocation = CurrentLocation + (NewLocation - CurrentLocation).GetSafeNormal() * MaxMoveDistance;
		}
	}
	
	if (bLookTarget == true)
	{
		TeleportTarget = (TeleportTarget != nullptr) ? TeleportTarget : FindTeleportTarget(Owner);
		if (TeleportTarget)
		{
			FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(NewLocation, TeleportTarget->GetActorLocation());
			NewRotation.Yaw = LookRotation.Yaw;
		}
	}

	Owner->TeleportTo(NewLocation, NewRotation);
}

AActor* URsAnimNotify_TeleportTo::FindTeleportTarget(AActor* Owner) const
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
