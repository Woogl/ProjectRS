// Copyright 2024 Team BH.


#include "RsAnimNotifyState_HitTrace.h"

#include "Rs/RsLogChannels.h"
#include "Rs/Battle/RsBattleLibrary.h"

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
		LastSocketTransform = MeshComp->GetSocketTransform(SocketName);
	}

	bStopTrace = false;
    HitTargets.Empty();
}

void URsAnimNotifyState_HitTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
   
    SCOPE_CYCLE_COUNTER(STAT_RsAnimNotifyState_HitTrace);
   
    if (!MeshComp || !MeshComp->GetWorld() || !LastSocketTransform.IsSet() || bStopTrace)
    {
       return;
    }
	
	float DeltaDistance = FVector::Dist(LastSocketTransform->GetLocation(), MeshComp->GetSocketLocation(SocketName));
	FVector ShapeExtent = GetCollisionShape().GetExtent();
	int32 SubstepNum = FMath::CeilToInt(DeltaDistance / FMath::Min3(ShapeExtent.X, ShapeExtent.Y, ShapeExtent.Z));
	SubstepNum = FMath::Min(SubstepNum, MaxSubstep);

	// Perform targeting from last transform to current transform without leaving any gaps.
	PerformTargeting(MeshComp, MeshComp->GetSocketTransform(SocketName));
	for (int32 i = 1; i <= SubstepNum; ++i)
	{
		float Alpha = static_cast<float>(i) / SubstepNum;
		FTransform SubstepTransform;
		SubstepTransform.Blend(LastSocketTransform.GetValue(), MeshComp->GetSocketTransform(SocketName), Alpha);
		PerformTargeting(MeshComp, SubstepTransform);
	}

	// Deal damage to each target
	for (TWeakObjectPtr<AActor> Target : Targets)
	{
		if (Target.IsValid() && !HitTargets.Contains(Target))
		{
			for (const FRsEffectCoefficient& EffectCoefficient : DamageContext.EffectCoefficients)
			{
				URsBattleLibrary::ApplyEffectCoefficient(MeshComp->GetOwner(), Target.Get(), EffectCoefficient);
			}
			HitTargets.Emplace(Target.Get());
			
			if (bStopTraceWhenFirstHit)
			{
				bStopTrace = true;
				break;
			}
		}
	}

	LastSocketTransform = MeshComp->GetSocketTransform(SocketName);
}
