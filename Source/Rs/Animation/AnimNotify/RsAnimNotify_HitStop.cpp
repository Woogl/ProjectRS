// Copyright 2024 Team BH.


#include "RsAnimNotify_HitStop.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_HitStop.h"

FString URsAnimNotify_HitStop::GetNotifyName_Implementation() const
{
	return FString(TEXT("AN_HitStop"));
}

void URsAnimNotify_HitStop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			if (UGameplayAbility* CurrentAbility = ASC->GetAnimatingAbility())
			{
				URsAbilityTask_HitStop* HitStopTask = URsAbilityTask_HitStop::StartHitStop(CurrentAbility, Duration);
				HitStopTask->ReadyForActivation();
			}
		}
	}
}
