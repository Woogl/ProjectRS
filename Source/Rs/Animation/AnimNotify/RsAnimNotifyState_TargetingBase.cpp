// Copyright 2024 Team BH.


#include "RsAnimNotifyState_TargetingBase.h"

#include "Rs/Targeting/RsTargetingLibrary.h"

void URsAnimNotifyState_TargetingBase::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

#if WITH_EDITOR
	SocketNames = MeshComp->GetAllSocketNames();
#endif // WITH_EDITOR
}

bool URsAnimNotifyState_TargetingBase::PerformTargeting(USkeletalMeshComponent* MeshComp, TArray<AActor*>& ResultActors)
{
	FTransform Transform = URsTargetingLibrary::GetSocketWorldTransform(MeshComp, SocketName, FTransform(RotationOffset, PositionOffset));
	URsTargetingLibrary::PerformTargeting(MeshComp->GetOwner(), Transform, FRsTargetingParams(Shape, Collision, Filter, Sorter), ResultActors);
	return !ResultActors.IsEmpty();
}
