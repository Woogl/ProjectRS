// Copyright 2025 Team BH.


#include "RsAnimNotifyState_MoveTo.h"

#include "Rs/Battle/RsBattleLibrary.h"
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
		// Use current lock on target.
		Target = URsBattleLibrary::GetLockOnTarget(Cast<APawn>(Owner));
		// Search new target if current lock on target is not available.
		if (!Target.IsValid())
		{
			TArray<AActor*> OutTargets;
			if (URsTargetingLibrary::PerformTargeting(Owner, Owner->GetTransform(), Shape, Collision, Filter, Sorter, OutTargets))
			{
				Target = OutTargets[0];
			}
		}
	}
	
	bShouldMove = Target.IsValid();

	if (bShouldMove)
	{
		StartLocation = Owner->GetActorLocation();
		if (PositionMode == ERsPositionMode::TowardTarget)
		{
			AcceptableRadiusSquared = AcceptableRadius * AcceptableRadius;
		}
	}
}

void URsAnimNotifyState_MoveTo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!bShouldMove)
	{
		return;
	}

	Elapsed += FrameDeltaTime;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	FVector CurrentLocation = Owner->GetActorLocation();
	FVector TargetLocation = CurrentLocation;

	switch (PositionMode)
	{
	case ERsPositionMode::LocalPosition_Target:
		if (Target.IsValid())
		{
			TargetLocation = Target->GetActorLocation() + Target->GetActorTransform().TransformVector(Position);
		}
		break;

	case ERsPositionMode::LocalPosition_Source:
		{
			TargetLocation = CurrentLocation + Owner->GetActorTransform().TransformVector(Position);
		}
		break;

	case ERsPositionMode::WorldPosition:
		{
			TargetLocation = Position;
		}
		break;

	case ERsPositionMode::TowardTarget:
		if (Target.IsValid())
		{
			TargetLocation = Target->GetActorLocation();
		}
		break;
	}

	float MaxDistSquared = MaxMoveDistance * MaxMoveDistance;
	float TotalDistSquared = FVector::DistSquared(StartLocation, TargetLocation);
	if (MaxMoveDistance > 0.f && TotalDistSquared > MaxDistSquared)
	{
		FVector Dir = (TargetLocation - StartLocation).GetSafeNormal();
		TargetLocation = StartLocation + Dir * MaxMoveDistance;
	}
	
	bool bReachedTarget = FVector::DistSquared(CurrentLocation, TargetLocation) <= AcceptableRadiusSquared;
	bool bReachedMax = MaxMoveDistance > 0.f && FVector::DistSquared(CurrentLocation, StartLocation) >= MaxDistSquared;
	if (bReachedTarget || bReachedMax)
	{
		bShouldMove = false;
		return;
	}

	FVector NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, GetNotifyProgress());
	Owner->SetActorLocation(NewLocation, true);
}

float URsAnimNotifyState_MoveTo::GetNotifyProgress() const
{
	return FMath::Clamp(Elapsed / FMath::Max(Duration, SMALL_NUMBER), 0.f, 1.f);
}
