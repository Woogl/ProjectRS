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

	AActor* Owner = MeshComp->GetOwner();	
	if (!PassCondition(Owner))
	{
		return;
	}
	
	// Use current lock on target.
	const AActor* TeleportTarget = URsBattleLibrary::GetLockOnTarget(Cast<APawn>(Owner));
	// Search new target if current lock on target is not available.
	if (!TeleportTarget)
	{
		TArray<AActor*> OutTargets;
		if (URsTargetingLibrary::PerformTargetingFromComponent(MeshComp, TargetingParams, OutTargets))
		{
			TeleportTarget = OutTargets[0];
		}
	}
	
	if (!TeleportTarget)
	{
		return;
	}

	const FTransform CurrentTransform = TeleportTarget->GetActorTransform();
	FVector NewLocation = CurrentTransform.TransformPosition(TargetLocalOffset);
	FRotator NewRotation = Owner->GetActorRotation();

	if (bLookTarget)
	{
		const FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(NewLocation, TeleportTarget->GetActorLocation());
		NewRotation.Yaw = LookRotation.Yaw;
	}

	Owner->TeleportTo(NewLocation, NewRotation);
}
