// Copyright 2025 Team BH.


#include "RsAnimNotifyState_EventToSelf.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"

URsAnimNotifyState_EventToSelf::URsAnimNotifyState_EventToSelf()
{
}

void URsAnimNotifyState_EventToSelf::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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
	if (Targets.IsEmpty())
	{
		return;
	}

	if (SourceASC)
	{
		FGameplayEventData Payload;
		Payload.EventTag = EventTag;
		Payload.EventMagnitude = TotalDuration;
		Payload.Instigator = MeshComp->GetOwner();
		SourceASC->HandleGameplayEvent(EventTag, &Payload);
	}
}

void URsAnimNotifyState_EventToSelf::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	// Should I targeting again?
}

void URsAnimNotifyState_EventToSelf::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (CurrentAbility)
	{
		CurrentAbility->RevertGameplayEvent(EventTag);
	}
}
