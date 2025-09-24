// Copyright 2024 Team BH.


#include "RsAnimNotify_EventToTarget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

URsAnimNotify_EventToTarget::URsAnimNotify_EventToTarget()
{
	bIsNativeBranchingPoint = true;
}

FString URsAnimNotify_EventToTarget::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		FString EventTagString = EventTag.ToString();
		return EventTagString.Replace(TEXT("AnimNotify."), TEXT(""));
	}
	return Super::GetNotifyName_Implementation();
}

void URsAnimNotify_EventToTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			for (AActor* Target : Targets)
			{
				FGameplayEventData Payload;
				Payload.EventTag = EventTag;
				Payload.Instigator = Owner;
				Payload.Target = Target;
				SourceASC->HandleGameplayEvent(EventTag, &Payload);
			}
		}
	}
}
