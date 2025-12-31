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
	
	// Use current lock on target.
	AActor* LocalTarget = URsBattleLibrary::GetLockOnTarget(Cast<APawn>(Owner));
	// Search new target if current lock on target is not available.
	if (!LocalTarget)
	{
		TArray<AActor*> OutTargets;
		if (URsTargetingLibrary::PerformTargeting(Owner, Owner->GetTransform(), TargetingParams, OutTargets))
		{
			LocalTarget = OutTargets[0];
		}
	}

	if (LocalTarget)
	{
		FMoveToRuntimeData NewData;
		NewData.MoveTarget = LocalTarget;
		NewData.bShouldMove = true;
		RuntimeDataMap.Add(MeshComp, NewData);
	}
}

void URsAnimNotifyState_MoveTo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	FMoveToRuntimeData* Data = RuntimeDataMap.Find(MeshComp);
	if (!Data || Data->bShouldMove == false || !Data->MoveTarget.IsValid())
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
	const FVector TargetLocation = Data->MoveTarget->GetActorLocation();
	const float Distance = FVector::Dist2D(CurrentLocation, TargetLocation);

	if (Distance < AcceptableRadius)
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
