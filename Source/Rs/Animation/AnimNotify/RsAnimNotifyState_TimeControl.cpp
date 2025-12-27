// Copyright 2025 Team BH.


#include "RsAnimNotifyState_TimeControl.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/World/TimeControl/RsTimeControlLibrary.h"

void URsAnimNotifyState_TimeControl::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!PassCondition(Owner))
	{
		return;
	}
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
	{
		URsTimeControlLibrary::RequestTimeDilation(ASC, GetFNameSafe(this), ERsTimeControlPriority::VFX, TimeDilation, TotalDuration, BlendTime);
	}
}
