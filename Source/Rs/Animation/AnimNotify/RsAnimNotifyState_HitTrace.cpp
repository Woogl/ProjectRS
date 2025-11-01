// Copyright 2024 Team BH.


#include "RsAnimNotifyState_HitTrace.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

DECLARE_STATS_GROUP(TEXT("RsAnimNotifyState"), STATGROUP_RSANIMNOTIFYSTATE, STATCAT_Advanced)
DECLARE_CYCLE_STAT(TEXT("RsAnimNotifyState_HitTrace"), STAT_RsAnimNotifyState_HitTrace, STATGROUP_RSANIMNOTIFYSTATE);

URsAnimNotifyState_HitTrace::URsAnimNotifyState_HitTrace()
{
	bIsNativeBranchingPoint = true;
	
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = true;
#endif // WITH_EDITORONLY_DATA
}

FString URsAnimNotifyState_HitTrace::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		FString EventTagString = EventTag.ToString();
		return EventTagString.Replace(TEXT("AnimNotify."), TEXT(""));
	}
	return Super::GetNotifyName_Implementation();
}

void URsAnimNotifyState_HitTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	LastWorldTransform.Reset();
	HitTargets.Reset();
	
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			if (UGameplayAbility* AnimatingAbility = ASC->GetAnimatingAbility())
			{
				CurrentAbility = Cast<URsGameplayAbility>(AnimatingAbility);
			}
		}

		TArray<AActor*> ResultActors;
		FRsTargetingParams Params(Shape, Collision, Filter, Sorter);
		FTransform WorldTransform = URsTargetingLibrary::GetSocketWorldTransform(MeshComp, SocketName, FTransform(RotationOffset, PositionOffset));
		bool bSuccess = URsTargetingLibrary::PerformTargeting(Owner, WorldTransform, Params, ResultActors);
		if (bSuccess == true)
		{
			SendHitEventToResults(MeshComp, ResultActors);
		}
	
		// Keep old socket transform for next tick.
		LastWorldTransform = WorldTransform;
	}
}

void URsAnimNotifyState_HitTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
   
    SCOPE_CYCLE_COUNTER(STAT_RsAnimNotifyState_HitTrace);
   
    if (!MeshComp || !MeshComp->GetWorld() || !LastWorldTransform.IsSet())
    {
       return;
    }

	TArray<AActor*> ResultActors;
	FRsTargetingParams Params(Shape, Collision, Filter, Sorter);
	FTransform WorldTransform = URsTargetingLibrary::GetSocketWorldTransform(MeshComp, SocketName, FTransform(RotationOffset, PositionOffset));
	bool bSuccess = URsTargetingLibrary::PerformTargetingWithSubsteps(MeshComp->GetOwner(), LastWorldTransform.GetValue(), WorldTransform, MaxSubsteps, Params, ResultActors);

	if (bSuccess == true)
	{
		SendHitEventToResults(MeshComp, ResultActors);
	}

	// Keep old socket transform for next tick.
	LastWorldTransform = WorldTransform;
}

void URsAnimNotifyState_HitTrace::SendHitEventToResults(USkeletalMeshComponent* MeshComp, TArray<AActor*> ResultActors)
{
	AActor* Owner = MeshComp->GetOwner();
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (!ASC)
	{
		return;
	}
	
	for (AActor* Target : ResultActors)
	{
		// Ignore already hit actors
		if (HitTargets.Contains(Target))
		{
			continue;
		}

		FGameplayEventData Payload;
		Payload.EventTag = EventTag;
		Payload.Instigator = Owner;
		Payload.Target = Target;
		FVector Start = MeshComp->GetSocketLocation(SocketName);
		FVector End = Target->GetActorLocation();
		FHitResult HitResult;
		FCollisionQueryParams Query;
		Query.AddIgnoredActor(Owner);
		Owner->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_RsAttack, Query);
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddHitResult(HitResult);
		Payload.ContextHandle = EffectContext;
		ASC->HandleGameplayEvent(EventTag, &Payload);
		HitTargets.Emplace(Target);
	}
}
