// Copyright 2024 Team BH.


#include "RsAnimNotifyState_TurnAround.h"

#include "Kismet/KismetMathLibrary.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotifyState_TurnAround::URsAnimNotifyState_TurnAround()
{
}

void URsAnimNotifyState_TurnAround::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp)
	{
		return;
	}
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	
	// Use current lock on target.
	TurnTarget = URsBattleLibrary::GetLockOnTarget(Cast<APawn>(Owner));
	// Search new target if current lock on target is not available.
	if (!TurnTarget.IsValid())
	{
		TArray<AActor*> OutTargets;
		if (URsTargetingLibrary::PerformTargeting(Owner, Owner->GetTransform(), Shape, Collision, Filter, Sorter, OutTargets))
		{
			TurnTarget = OutTargets[0];
		}
	}
	
	bShouldTurn = TurnTarget.IsValid();
}

void URsAnimNotifyState_TurnAround::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (bShouldTurn == false || MeshComp == nullptr)
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

	if (bShouldTurn == true && FMath::IsNearlyEqual(NewRotation.Yaw, TargetRotation.Yaw))
	{
		bShouldTurn = false;
	}
}
