// Copyright 2024 Team BH.


#include "RsAnimNotifyState_TurnAround.h"

#include "Kismet/KismetMathLibrary.h"

URsAnimNotifyState_TurnAround::URsAnimNotifyState_TurnAround()
{
}

void URsAnimNotifyState_TurnAround::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	bTurnComplete = false;
}

void URsAnimNotifyState_TurnAround::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (bTurnComplete == true || MeshComp == nullptr)
	{
		return;
	}
	
	UWorld* World = MeshComp->GetWorld();
	if (World == nullptr)
	{
		return;
	}

	AActor* OwnerActor = MeshComp->GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}

	if (Targets.IsEmpty() || Targets[0].IsValid() == false)
	{
		return;
	}

	FRotator CurrentRotation = OwnerActor->GetActorRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(OwnerActor->GetActorLocation(), Targets[0]->GetActorLocation());

	float NewYaw = FMath::FixedTurn(CurrentRotation.Yaw, TargetRotation.Yaw, MaxTurnAroundSpeed * FrameDeltaTime);

	FRotator NewRotation = FRotator(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll);
	OwnerActor->SetActorRotation(NewRotation);

	if (bTurnComplete == false && FMath::IsNearlyEqual(NewRotation.Yaw, TargetRotation.Yaw, 1.0f))
	{
		bTurnComplete = true;
	}
}
