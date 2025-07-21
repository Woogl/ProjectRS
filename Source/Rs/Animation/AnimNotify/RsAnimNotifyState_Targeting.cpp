// Copyright 2024 Team BH.


#include "RsAnimNotifyState_Targeting.h"

#include "Rs/Targeting/RsTargetingLibrary.h"


void URsAnimNotifyState_Targeting::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

#if WITH_EDITOR
	SocketNames = MeshComp->GetAllSocketNames();
#endif // WITH_EDITOR
	
	PerformTargeting(MeshComp);
}

void URsAnimNotifyState_Targeting::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
}

bool URsAnimNotifyState_Targeting::PerformTargeting(USkeletalMeshComponent* MeshComp)
{
	if (!MeshComp)
	{
		return false;
	}

	Targets.Reset();
	
	FTransform SourceTransform = SocketName.IsValid() ? MeshComp->GetSocketTransform(SocketName) : MeshComp->GetComponentTransform();
	if (MeshComp->GetOwner())
	{
		TArray<AActor*> OutActors;
		if (URsTargetingLibrary::PerformTargeting(MeshComp->GetOwner(), SourceTransform, Collision, Filter, Sorter, OutActors))
		{
			Targets = OutActors;
		}
	}

	return Targets.Num() > 0;
}
