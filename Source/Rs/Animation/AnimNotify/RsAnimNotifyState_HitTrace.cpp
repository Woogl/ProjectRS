// Copyright 2024 Team BH.


#include "RsAnimNotifyState_HitTrace.h"

#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

DECLARE_STATS_GROUP(TEXT("RsAnimNotifyState"), STATGROUP_RSANIMNOTIFYSTATE, STATCAT_Advanced)
DECLARE_CYCLE_STAT(TEXT("RsAnimNotifyState_HitTrace"), STAT_RsAnimNotifyState_HitTrace, STATGROUP_RSANIMNOTIFYSTATE);

URsAnimNotifyState_HitTrace::URsAnimNotifyState_HitTrace()
{
	bSortByDistance = false;
}

FString URsAnimNotifyState_HitTrace::GetNotifyName_Implementation() const
{
	if (DamageContext.DamageEventTag.IsValid())
	{
		FString EventTagString = DamageContext.DamageEventTag.ToString();
		return EventTagString.Replace(TEXT("AnimNotify."), TEXT(""));
	}
	return Super::GetNotifyName_Implementation();
}

void URsAnimNotifyState_HitTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp)
	{
		LastSocketTransform = MeshComp->GetSocketTransform(SocketName);
	}

	bStopTrace = false;
    HitTargets.Reset();
}

void URsAnimNotifyState_HitTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
   
    SCOPE_CYCLE_COUNTER(STAT_RsAnimNotifyState_HitTrace);
   
    if (!MeshComp || !MeshComp->GetWorld() || !LastSocketTransform.IsSet() || bStopTrace)
    {
       return;
    }

	// Get socket transform.
	FTransform CurrentSocketTransform = SocketName.IsValid() ? MeshComp->GetSocketTransform(SocketName) : MeshComp->GetComponentTransform();
	CurrentSocketTransform.SetLocation(CurrentSocketTransform.GetLocation() + MeshComp->GetComponentTransform().TransformVector(PositionOffset));
	CurrentSocketTransform.SetRotation(RotationOffset.Quaternion() * CurrentSocketTransform.GetRotation());

	FRsTargetingCollision Collision(CollisionObjectTypes, ShapeType, HalfExtent, Radius, HalfHeight);
	FRsTargetingFilter Filter(bIncludeSelf, bIncludeFriendlyTeam, bIncludeHostileTeam, MaxTargetCount, TargetRequirements, HitTargets);
	FRsTargetingSorter Sorter(bSortByDistance);

	TArray<AActor*> ResultActors;
	if (URsTargetingLibrary::PerformTargetingWithSubsteps(MeshComp->GetOwner(), LastSocketTransform.GetValue(), CurrentSocketTransform, MaxSubsteps, Collision, Filter, Sorter, ResultActors))
	{
		// Deal damage to each target.
		for (AActor* Target : ResultActors)
		{
			URsBattleLibrary::ApplyDamageContext(MeshComp->GetOwner(), Target, DamageContext);
			HitTargets.Emplace(Target);
			
			if (bStopTraceWhenFirstHit)
			{
				bStopTrace = true;
				break;
			}
		}
	}

	// Keep old socket transform for next tick.
	LastSocketTransform = MeshComp->GetSocketTransform(SocketName);
}
