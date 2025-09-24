// Copyright 2025 Team BH.


#include "RsAnimNotifyState_EventToTarget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"

URsAnimNotifyState_EventToTarget::URsAnimNotifyState_EventToTarget()
{
	bIsNativeBranchingPoint = true;
}

FString URsAnimNotifyState_EventToTarget::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		FString EventTagString = EventTag.ToString();
		return EventTagString.Replace(TEXT("AnimNotify."), TEXT(""));
	}
	return Super::GetNotifyName_Implementation();
}

void URsAnimNotifyState_EventToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			CurrentAbility = OwnerASC->GetAnimatingAbility();
			for (AActor* Target : Targets)
			{
				FGameplayEventData Payload;
				Payload.EventTag = EventTag;
				Payload.Instigator = Owner;
				Payload.Target = Target;
				OwnerASC->HandleGameplayEvent(EventTag, &Payload);
			}
		}
	}
}

void URsAnimNotifyState_EventToTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
