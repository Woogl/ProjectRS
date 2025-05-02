// Copyright 2024 Team BH.


#include "RsAnimNotifyState_HitTrace.h"

DECLARE_STATS_GROUP(TEXT("RsAnimNotifyState"), STATGROUP_RSANIMNOTIFYSTATE, STATCAT_Advanced)
DECLARE_CYCLE_STAT(TEXT("RsAnimNotifyState_HitTrace"), STAT_RsAnimNotifyState_HitTrace, STATGROUP_RSANIMNOTIFYSTATE);

URsAnimNotifyState_HitTrace::URsAnimNotifyState_HitTrace()
{
	bSortByDistance = false;
}

void URsAnimNotifyState_HitTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp)
	{
		PreviousSocketTransform = MeshComp->GetSocketTransform(SocketName);
	}

   HitTargets.Empty();
}

void URsAnimNotifyState_HitTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
   
    SCOPE_CYCLE_COUNTER(STAT_RsAnimNotifyState_HitTrace);
   
    if (!MeshComp || !MeshComp->GetWorld() || !PreviousSocketTransform.IsSet())
    {
       return;
    }
   
    // TODO: Fix tunneling & Multi-thread
   
    PerformTargeting(MeshComp);

	for (TWeakObjectPtr<AActor> Target : Targets)
	{
		if (Target.IsValid() && !HitTargets.Contains(Target))
		{
			// TODO: Apply damage
			HitTargets.AddUnique(Target.Get());
		}
	}
}
