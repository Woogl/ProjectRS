// Copyright 2025 Team BH.


#include "RsAnimNotifyState_GrantTags.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotifyState_GrantTags::URsAnimNotifyState_GrantTags()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotifyState_GrantTags::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!PassCondition(Owner))
	{
		return;
	}
	
	if (TargetType == ERsTargetType::Source)
	{
		if (!Tags.IsValid())
		{
			return;
		}
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			ASC->AddLooseGameplayTags(Tags);
		}
	}
	else if (TargetType == ERsTargetType::Target)
	{
		TArray<AActor*> OutTargets;
		URsTargetingLibrary::PerformTargetingFromComponent(MeshComp, TargetingParams, OutTargets);
		if (!Tags.IsValid())
		{
			return;
		}
		for (AActor* Target : OutTargets)
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target))
			{
				ASC->AddLooseGameplayTags(Tags);
			}
		}
	}
}

void URsAnimNotifyState_GrantTags::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
	{
		ASC->RemoveLooseGameplayTags(Tags);
	}
}
