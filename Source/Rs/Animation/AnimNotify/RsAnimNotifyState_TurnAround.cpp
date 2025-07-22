// Copyright 2024 Team BH.


#include "RsAnimNotifyState_TurnAround.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "Rs/Player/RsPlayerController.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotifyState_TurnAround::URsAnimNotifyState_TurnAround()
{
}

void URsAnimNotifyState_TurnAround::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	bTurnComplete = false;
	TurnTarget.Reset();
	
	TurnTarget = FindTurnTarget(MeshComp->GetOwner());
}

void URsAnimNotifyState_TurnAround::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (bTurnComplete == true || MeshComp == nullptr)
	{
		return;
	}
	
	UWorld* World = MeshComp->GetWorld();
	if (!World)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	if (!TurnTarget.IsValid())
	{
		return;
	}

	FRotator CurrentRotation = Owner->GetActorRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), TurnTarget->GetActorLocation());

	float NewYaw = FMath::FixedTurn(CurrentRotation.Yaw, TargetRotation.Yaw, MaxTurnAroundSpeed * FrameDeltaTime);

	FRotator NewRotation = FRotator(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll);
	Owner->SetActorRotation(NewRotation);

	if (bTurnComplete == false && FMath::IsNearlyEqual(NewRotation.Yaw, TargetRotation.Yaw))
	{
		bTurnComplete = true;
	}
}

AActor* URsAnimNotifyState_TurnAround::FindTurnTarget(AActor* Owner) const
{
	if (!Owner)
	{
		return nullptr;
	}

	AActor* FoundTarget = nullptr;
	
	if (bUseLockOnTargetFirst)
	{
		if (ACharacter* Character = Cast<ACharacter>(Owner))
		{
			if (AController* Controller = Character->GetController())
			{
				// PlayerCharacter: LockOnComponent
				if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(Controller))
				{
					if (URsLockOnComponent* LockOnComponent = RsPlayerController->GetLockOnComponent())
					{
						FoundTarget = LockOnComponent->GetLockOnTarget();
					}
				}
				// EnemyCharacter: Blackboard
				else if (AAIController* AIController = Cast<AAIController>(Controller))
				{
					if (UBlackboardComponent* BBComponent = AIController->GetBlackboardComponent())
					{
						if (UObject* BBObject = BBComponent->GetValueAsObject(TEXT("TargetActor")))
						{
							FoundTarget = Cast<AActor>(BBObject);
						}
					}
				}
			}
		}
	}

	// Fallback: Targeting
	if (!FoundTarget)
	{
		TArray<AActor*> OutActors;
		if (URsTargetingLibrary::PerformTargeting(Owner, Owner->GetActorTransform(), Collision, Filter, Sorter, OutActors))
		{
			FoundTarget = OutActors[0];
		}
	}

	return FoundTarget;
}
