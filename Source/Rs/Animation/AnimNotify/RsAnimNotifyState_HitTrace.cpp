﻿// Copyright 2024 Team BH.


#include "RsAnimNotifyState_HitTrace.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

DECLARE_STATS_GROUP(TEXT("RsAnimNotifyState"), STATGROUP_RSANIMNOTIFYSTATE, STATCAT_Advanced)
DECLARE_CYCLE_STAT(TEXT("RsAnimNotifyState_HitTrace"), STAT_RsAnimNotifyState_HitTrace, STATGROUP_RSANIMNOTIFYSTATE);

URsAnimNotifyState_HitTrace::URsAnimNotifyState_HitTrace()
{
}

FString URsAnimNotifyState_HitTrace::GetNotifyName_Implementation() const
{
	if (DamageEvent.IsValid())
	{
		FString EventTagString = DamageEvent.ToString();
		return EventTagString.Replace(TEXT("AnimNotify."), TEXT(""));
	}
	return Super::GetNotifyName_Implementation();
}

void URsAnimNotifyState_HitTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		LastSocketTransform = MeshComp->GetSocketTransform(SocketName);
		
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			if (UGameplayAbility* AnimatingAbility = ASC->GetAnimatingAbility())
			{
				CurrentAbility = Cast<URsGameplayAbility_Attack>(AnimatingAbility);
			}
		}
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

	TArray<AActor*> ResultActors;
	if (URsTargetingLibrary::PerformTargetingWithSubsteps(MeshComp->GetOwner(), LastSocketTransform.GetValue(), CurrentSocketTransform, MaxSubsteps, Collision, Filter, Sorter, ResultActors))
	{
		// Deal damage to each target.
		for (AActor* Target : ResultActors)
		{
			if (!HitTargets.Contains(Target))
			{
				CurrentAbility->ApplyDamageEvent(DamageEvent, Target);
				HitTargets.Emplace(Target);
			
				if (bStopTraceWhenFirstHit)
				{
					bStopTrace = true;
					break;
				}
			}
		}
	}

	// Keep old socket transform for next tick.
	LastSocketTransform = MeshComp->GetSocketTransform(SocketName);
}
