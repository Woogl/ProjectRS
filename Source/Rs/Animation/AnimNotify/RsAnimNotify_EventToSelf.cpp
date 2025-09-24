// Copyright 2024 Team BH.


#include "RsAnimNotify_EventToSelf.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

URsAnimNotify_EventToSelf::URsAnimNotify_EventToSelf()
{
	bIsNativeBranchingPoint = true;
}

FString URsAnimNotify_EventToSelf::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		FString EventTagString = EventTag.ToString();
		return EventTagString.Replace(TEXT("AnimNotify."), TEXT(""));
	}
	return Super::GetNotifyName_Implementation();
}

void URsAnimNotify_EventToSelf::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			FGameplayEventData Payload;
			Payload.EventTag = EventTag;
			Payload.Instigator = Owner;
			Payload.Target = Owner;
			ASC->HandleGameplayEvent(EventTag, &Payload);
		}
	}
}
