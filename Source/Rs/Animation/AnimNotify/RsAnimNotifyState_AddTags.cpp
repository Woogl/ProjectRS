// Copyright 2025 Team BH.


#include "RsAnimNotifyState_AddTags.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

URsAnimNotifyState_AddTags::URsAnimNotifyState_AddTags()
{
}

void URsAnimNotifyState_AddTags::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!Tags.IsValid())
	{
		return;
	}
	
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
	{
		ASC->AddLooseGameplayTags(Tags);
	}
}

void URsAnimNotifyState_AddTags::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (!Tags.IsValid())
	{
		return;
	}
	
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
	{
		ASC->RemoveLooseGameplayTags(Tags);
	}
}
