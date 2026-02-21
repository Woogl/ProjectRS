// Copyright 2025 Team BH.


#include "RsAnimNotify_SendEvent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotify_SendEvent::URsAnimNotify_SendEvent()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotify_SendEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!PassCondition(Owner))
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	
	if (TargetType == ERsTargetType::Source)
	{
		if (EventTag.IsValid() && ASC)
		{
			FGameplayEventData Payload;
			Payload.EventTag = EventTag;
			Payload.Instigator = Owner;
			Payload.Target = Owner;
			ASC->HandleGameplayEvent(EventTag, &Payload);
		}
	}
	
	else if (TargetType == ERsTargetType::Target)
	{
		TArray<AActor*> OutTargets;
		URsTargetingLibrary::PerformTargetingFromComponent(MeshComp, TargetingParams, OutTargets);
		
		if (EventTag.IsValid() && ASC)
		{
			for (AActor* Target : OutTargets)
			{
				FGameplayEventData Payload;
				Payload.EventTag = EventTag;
				Payload.Instigator = Owner;
				Payload.Target = Target;
				ASC->HandleGameplayEvent(EventTag, &Payload);
			}
		}
	}
}
