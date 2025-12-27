// Copyright 2025 Team BH.


#include "RsAnimNotify_TeleportTo.h"

#include "Kismet/KismetMathLibrary.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotify_TeleportTo::URsAnimNotify_TeleportTo()
{
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
		TArray<AActor*> OutTargets;
		if (URsTargetingLibrary::PerformTargeting(Owner, Owner->GetTransform(), TargetingParams, OutTargets))
		{
			TeleportTarget = OutTargets[0];
		}
	}
	
	if (bLookTarget && TeleportTarget)
	{
		FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(NewLocation, TeleportTarget->GetActorLocation());
		NewRotation.Yaw = LookRotation.Yaw;
	}

	Owner->TeleportTo(NewLocation, NewRotation);
}
