// Copyright 2024 Team BH.


#include "RsAnimNotifyState_HitStop.h"

#include "GameplayEffectTypes.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_PauseMontage.h"

void URsAnimNotifyState_HitStop::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (OwnerASC.IsValid())
	{
		if (URsAbilitySystemComponent* RsASC = Cast<URsAbilitySystemComponent>(OwnerASC))
		{
			RsASC->OnDealDamage.AddUniqueDynamic(this, &ThisClass::HandleDealDamage);
		}
	}
}

void URsAnimNotifyState_HitStop::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (OwnerASC.IsValid())
	{
		if (URsAbilitySystemComponent* RsASC = Cast<URsAbilitySystemComponent>(OwnerASC))
		{
			RsASC->OnDealDamage.RemoveDynamic(this, &ThisClass::HandleDealDamage);
		}
	}
}

void URsAnimNotifyState_HitStop::HandleDealDamage(UAbilitySystemComponent* TargetASC, FGameplayEffectSpecHandle DamageEffectHandle)
{
	if (CurrentAbility.IsValid())
	{
		PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(CurrentAbility.Get(), Duration);
		PauseMontageTask->ReadyForActivation();
	}
}
