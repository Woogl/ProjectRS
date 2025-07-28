// Copyright 2025 Team BH.


#include "RsAnimNotifyState_MoveTo.h"

#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Character/RsCharacterBase.h"

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
		if (ARsCharacterBase* Character = Cast<ARsCharacterBase>(Owner))
		{
			if (!bKeepExistingTarget)
			{
				Target = URsBattleLibrary::AcquireTargetByControllerType(Character, Shape, Collision, Filter, Sorter);
			}
			else
			{
				Target = URsBattleLibrary::GetLockOnTarget(Character);
			}
		}
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

float URsAnimNotifyState_MoveTo::GetNotifyProgress() const
{
	return FMath::Clamp(Elapsed / FMath::Max(Duration, SMALL_NUMBER), 0.f, 1.f);
}
