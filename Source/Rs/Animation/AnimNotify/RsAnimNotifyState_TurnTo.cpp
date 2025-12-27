// Copyright 2024 Team BH.


#include "RsAnimNotifyState_TurnTo.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

void URsAnimNotifyState_TurnTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	if (!PassCondition(Owner))
	{
		return;
	}
	
	FTurnToRuntimeData& Data = RuntimeDataMap.Add(MeshComp);
	
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
	
	Data.bShouldTurn = Data.TurnTarget.IsValid();
}

void URsAnimNotifyState_TurnTo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	FTurnToRuntimeData* Data = RuntimeDataMap.Find(MeshComp);
	if (!Data || Data->bShouldTurn == false || !Data->TurnTarget.IsValid())
	{
		return;
	}
	
	AActor* Owner = MeshComp->GetOwner();
	const FRotator CurrentRotation = Owner->GetActorRotation();
	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), Data->TurnTarget->GetActorLocation());

	float RotationSpeed = 200.f;
	if (const ACharacter* Character = Cast<ACharacter>(Owner))
	{
		RotationSpeed = Character->GetCharacterMovement()->RotationRate.Yaw;
	}
	
	float NewYaw = FMath::FixedTurn(CurrentRotation.Yaw, TargetRotation.Yaw, RotationSpeed * FrameDeltaTime);
	Owner->SetActorRotation(FRotator(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll));

	const float YawDelta = FMath::Abs(FMath::FindDeltaAngleDegrees(NewYaw, TargetRotation.Yaw));
	if (YawDelta <= 1.f)
	{
		Data->bShouldTurn = false;
	}
}

void URsAnimNotifyState_TurnTo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	RuntimeDataMap.Remove(MeshComp);
}
