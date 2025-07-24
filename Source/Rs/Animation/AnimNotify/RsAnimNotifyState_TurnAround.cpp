// Copyright 2024 Team BH.


#include "RsAnimNotifyState_TurnAround.h"

#include "Kismet/KismetMathLibrary.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Character/RsCharacterBase.h"
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

	if (ARsCharacterBase* Character = Cast<ARsCharacterBase>(MeshComp->GetOwner()))
	{
		TurnTarget = FindTurnTarget(Character);
	}
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

AActor* URsAnimNotifyState_TurnAround::FindTurnTarget(ARsCharacterBase* Owner) const
{
	if (!Owner)
	{
		return nullptr;
	}

	AActor* FoundTarget = nullptr;
	
	if (bUseLockOnTargetFirst)
	{
		FoundTarget = URsBattleLibrary::GetLockOnTarget(Owner);
	}

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
