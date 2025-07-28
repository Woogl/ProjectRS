// Copyright 2025 Team BH.


#include "RsAnimNotify_TeleportTo.h"

#include "Kismet/KismetMathLibrary.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Character/RsCharacterBase.h"

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
	
	AActor* TeleportTarget;
	ARsCharacterBase* Character = Cast<ARsCharacterBase>(Owner);
	if (!bKeepExistingTarget)
	{
		TeleportTarget = URsBattleLibrary::AcquireTargetByControllerType(Character, Shape, Collision, Filter, Sorter);
	}
	else
	{
		TeleportTarget = URsBattleLibrary::GetLockOnTarget(Character);
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
	
	if (bLookTarget && TeleportTarget)
	{
		FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(NewLocation, TeleportTarget->GetActorLocation());
		NewRotation.Yaw = LookRotation.Yaw;
	}

	Owner->TeleportTo(NewLocation, NewRotation);
}
