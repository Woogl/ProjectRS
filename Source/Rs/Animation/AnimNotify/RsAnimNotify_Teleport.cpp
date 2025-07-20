// Copyright 2025 Team BH.


#include "RsAnimNotify_Teleport.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "Rs/Player/RsPlayerController.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotify_Teleport::URsAnimNotify_Teleport()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotify_Teleport::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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

	FVector NewLocation = Owner->GetActorLocation();
	FRotator NewRotation = Owner->GetActorRotation();
	AActor* TeleportTarget = nullptr;

	switch (PositionMode)
	{
	case ERsTeleportPosition::SourceLocalPosition:
		NewLocation += Owner->GetActorTransform().TransformVector(Position);
		break;

	case ERsTeleportPosition::WorldPosition:
		NewLocation = Position;
		break;

	case ERsTeleportPosition::TargetLocalPosition:
		TeleportTarget = FindTeleportTarget(Owner);
		if (TeleportTarget)
		{
			FRotator TargetRotation = TeleportTarget->GetActorRotation();
			NewLocation = TeleportTarget->GetActorLocation() + TargetRotation.RotateVector(Position);
		}
		break;
	}

	if (bLookTarget == true)
	{
		TeleportTarget = (TeleportTarget != nullptr) ? TeleportTarget : FindTeleportTarget(Owner);
		if (TeleportTarget)
		{
			const FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(NewLocation, TeleportTarget->GetActorLocation());
			NewRotation.Yaw = LookRotation.Yaw;
		}
	}

	Owner->TeleportTo(NewLocation, NewRotation);
}

AActor* URsAnimNotify_Teleport::FindTeleportTarget(AActor* Owner) const
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
