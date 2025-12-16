// Copyright 2024 Team BH.


#include "RsAnimNotify_TargetingBase.h"

#include "Rs/Targeting/RsTargetingLibrary.h"


void URsAnimNotify_TargetingBase::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

#if WITH_EDITOR
	SocketNames = MeshComp->GetAllSocketNames();
#endif // WITH_EDITOR
}

bool URsAnimNotify_TargetingBase::PerformTargeting(USkeletalMeshComponent* MeshComp)
{
	if (!MeshComp)
	{
		return false;
	}

	Targets.Reset();
	
	if (AActor* Owner = MeshComp->GetOwner())
	{
		TArray<AActor*> OutActors;
		FTransform WorldTransform = URsTargetingLibrary::GetSocketWorldTransform(MeshComp, SocketName, FTransform(RotationOffset, PositionOffset));
		FRsTargetingParams Params(Shape, Collision, Filter, Sorter);
		if (URsTargetingLibrary::PerformTargeting(Owner, WorldTransform, Params, OutActors))
		{
			Targets = OutActors;
		}
	}

	return Targets.Num() > 0;
}
