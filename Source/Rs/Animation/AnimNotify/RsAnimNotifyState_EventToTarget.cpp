// Copyright 2025 Team BH.


#include "RsAnimNotifyState_EventToTarget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"

URsAnimNotifyState_EventToTarget::URsAnimNotifyState_EventToTarget()
{
}

void URsAnimNotifyState_EventToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (SourceASC)
	{
		CurrentAbility = Cast<URsGameplayAbility>(SourceASC->GetAnimatingAbility());
	}
	
	PerformTargeting(MeshComp, Targets);

	if (SourceASC)
	{
		for (AActor* Target : Targets)
		{
			FGameplayEventData Payload;
			Payload.EventTag = EventTag;
			Payload.Instigator = Owner;
			Payload.Target = Target;
			Payload.EventMagnitude = TotalDuration;
			SourceASC->HandleGameplayEvent(EventTag, &Payload);
			
			//Filter.ActorsToIgnore.Add(Target);
		}
	}
}

void URsAnimNotifyState_EventToTarget::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	// Should I targeting again?
}

void URsAnimNotifyState_EventToTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (CurrentAbility)
	{
		CurrentAbility->RevertGameplayEvent(EventTag);
	}
}
