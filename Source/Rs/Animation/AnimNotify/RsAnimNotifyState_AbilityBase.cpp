// Copyright 2024 Team BH.


#include "RsAnimNotifyState_AbilityBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void URsAnimNotifyState_AbilityBase::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
		if (OwnerASC)
		{
			CurrentAbility = OwnerASC->GetAnimatingAbility();
		}
	}
}
