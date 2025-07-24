// Copyright 2025 Team BH.


#include "RsAnimNotify_TeleportTo.h"

#include "Kismet/KismetMathLibrary.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Character/RsCharacterBase.h"
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
		TeleportTarget = (TeleportTarget != nullptr) ? TeleportTarget : FindTeleportTarget(Cast<ARsCharacterBase>(Owner));
		if (TeleportTarget)
		{
			FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(NewLocation, TeleportTarget->GetActorLocation());
			NewRotation.Yaw = LookRotation.Yaw;
		}
	}

	Owner->TeleportTo(NewLocation, NewRotation);
}

AActor* URsAnimNotify_TeleportTo::FindTeleportTarget(ARsCharacterBase* Owner) const
{
	if (!Owner)
	{
		return nullptr;
	}
	
	AActor* TeleportTarget = nullptr;
	
	if (bUseLockOnTargetFirst)
	{
		TeleportTarget = URsBattleLibrary::GetLockOnTarget(Owner);
	}

	if (!TeleportTarget)
	{
		TArray<AActor*> OutActors;
		if (URsTargetingLibrary::PerformTargeting(Owner, Owner->GetActorTransform(), Collision, Filter, Sorter, OutActors))
		{
			TeleportTarget = OutActors[0];
		}
	}

	return TeleportTarget;
}
