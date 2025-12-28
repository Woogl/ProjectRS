// Copyright 2025 Team BH.


#include "RsAnimNotifyState_MoveTo.h"

#include "GameFramework/Character.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

void URsAnimNotifyState_MoveTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	if (!PassCondition(Owner))
	{
		return;
	}
	
	FMoveToRuntimeData& Data = RuntimeDataMap.Add(MeshComp);
	
	// Use current lock on target.
	Data.TurnTarget = URsBattleLibrary::GetLockOnTarget(Cast<APawn>(Owner));
	// Search new target if current lock on target is not available.
	if (!Data.TurnTarget.IsValid())
	{
		TArray<AActor*> OutTargets;
		if (URsTargetingLibrary::PerformTargeting(Owner, Owner->GetTransform(), TargetingParams, OutTargets))
		{
			Data.TurnTarget = OutTargets[0];
		}
	}
	
	Data.bShouldMove = Data.TurnTarget.IsValid();
}

void URsAnimNotifyState_MoveTo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	FMoveToRuntimeData* Data = RuntimeDataMap.Find(MeshComp);
	if (!Data || Data->bShouldMove == false || !Data->TurnTarget.IsValid())
	{
		return;
	}
	
	AActor* Owner = MeshComp->GetOwner();
	ACharacter* Character = Cast<ACharacter>(Owner);
	if (!Character)
	{
		// Owner must be character.
		return;
	}
	
	const FVector CurrentLocation = Owner->GetActorLocation();
	const FVector TargetLocation = CurrentLocation;
	const float Distance = FVector::Dist2D(CurrentLocation, TargetLocation);

	if (Distance <= Data->AcceptableRadius)
	{
		Data->bShouldMove = false;
		return;
	}

	FVector Direction = (TargetLocation - CurrentLocation);
	Direction.Z = 0.f;
	Direction.Normalize();
	
	Character->AddMovementInput(Direction, 1);
}

void URsAnimNotifyState_MoveTo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	RuntimeDataMap.Remove(MeshComp);
}
