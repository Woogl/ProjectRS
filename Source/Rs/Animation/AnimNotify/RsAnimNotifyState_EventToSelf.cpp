// Copyright 2025 Team BH.


#include "RsAnimNotifyState_EventToSelf.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"


URsAnimNotifyState_EventToSelf::URsAnimNotifyState_EventToSelf()
{
	bIsNativeBranchingPoint = true;
}

FString URsAnimNotifyState_EventToSelf::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		FString EventTagString = EventTag.ToString();
		return EventTagString.Replace(TEXT("AnimNotify."), TEXT(""));
	}
	return Super::GetNotifyName_Implementation();
}

void URsAnimNotifyState_EventToSelf::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		FGameplayEventData Payload;
		Payload.EventTag = EventTag;
		Payload.Instigator = Owner;
		Payload.Target = Owner;
		OwnerASC->HandleGameplayEvent(EventTag, &Payload);
	}
}

void URsAnimNotifyState_EventToSelf::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (CurrentAbility.IsValid())
	{
		if (URsGameplayAbility* RsAbility = Cast<URsGameplayAbility>(CurrentAbility))
		{
			RsAbility->CancelAbilityEvent(EventTag);
		}
	}
}
