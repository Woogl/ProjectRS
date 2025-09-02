// Copyright 2025 Team BH.


#include "RsAnimNotify_TeleportTo.h"

#include "Kismet/KismetMathLibrary.h"
#include "Rs/Battle/RsBattleLibrary.h"
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
	FVector NewLocation = CurrentLocation;
	FRotator NewRotation = Owner->GetActorRotation();
	
	// Use current lock on target.
	AActor* TeleportTarget = URsBattleLibrary::GetLockOnTarget(Cast<APawn>(Owner));
	// Search new target if current lock on target is not available.
	if (!TeleportTarget)
	{
		FRsTargetingParams Params(Shape, Collision, Filter, Sorter);
		TArray<AActor*> OutTargets;
		if (URsTargetingLibrary::PerformTargeting(Owner, Owner->GetTransform(), Params, OutTargets))
		{
			TeleportTarget = OutTargets[0];
		}
	}
	
	if (PositionMode == ERsPositionMode::LocalPosition_Target && TeleportTarget)
	{
		NewLocation = TeleportTarget->GetActorLocation() + TeleportTarget->GetActorTransform().TransformVector(Position);
	}
	else if (PositionMode == ERsPositionMode::LocalPosition_Source)
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

	if (PositionMode == ERsPositionMode::TowardTarget && TeleportTarget)
	{
		if (FVector::DistSquared(CurrentLocation, NewLocation) > AcceptableRadius * AcceptableRadius)
		{
			FVector Direction = (NewLocation - CurrentLocation).GetSafeNormal();
			NewLocation = TeleportTarget->GetActorLocation() - Direction * AcceptableRadius;
		}
	}
	
	if (bLookTarget && TeleportTarget)
	{
		FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(NewLocation, TeleportTarget->GetActorLocation());
		NewRotation.Yaw = LookRotation.Yaw;
	}

	Owner->TeleportTo(NewLocation, NewRotation);
}
