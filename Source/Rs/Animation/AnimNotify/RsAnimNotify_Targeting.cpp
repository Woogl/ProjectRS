// Copyright 2024 Team BH.


#include "RsAnimNotify_Targeting.h"

#include "Rs/Targeting/RsTargetingLibrary.h"


void URsAnimNotify_Targeting::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	PerformTargeting(MeshComp);

#if WITH_EDITOR
	SocketNames = MeshComp->GetAllSocketNames();
#endif // WITH_EDITOR
}

bool URsAnimNotify_Targeting::PerformTargeting(USkeletalMeshComponent* MeshComp)
{
	if (!MeshComp)
	{
		return false;
	}

	Targets.Reset();
	
	FTransform SourceTransform = SocketName.IsValid() ? MeshComp->GetSocketTransform(SocketName) : MeshComp->GetComponentTransform();
	if (MeshComp->GetOwner())
	{
		FRsTargetingParams Params(Shape, Collision, Filter, Sorter);
		TArray<AActor*> OutActors;
		if (URsTargetingLibrary::PerformTargeting(MeshComp->GetOwner(), SourceTransform, Params, OutActors))
		{
			Targets = OutActors;
		}
	}

	return Targets.Num() > 0;
}
