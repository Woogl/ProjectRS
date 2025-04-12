// Copyright 2024 Team BH.


#include "RsAnimNotifyState_HitStop.h"

#include "AbilitySystemGlobals.h"
#include "GameplayEffectTypes.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_PauseMontage.h"
#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"

void URsAnimNotifyState_HitStop::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			RsAbilitySystemComponent = Cast<URsAbilitySystemComponent>(ASC);
			if (RsAbilitySystemComponent.IsValid())
			{
				RsAbilitySystemComponent->OnDealDamage.AddUniqueDynamic(this, &ThisClass::HandleDealDamage);
			}
		}
	}
}

void URsAnimNotifyState_HitStop::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (RsAbilitySystemComponent.IsValid())
	{
		RsAbilitySystemComponent->OnDealDamage.RemoveDynamic(this, &ThisClass::HandleDealDamage);
	}
}

void URsAnimNotifyState_HitStop::HandleDealDamage(UAbilitySystemComponent* TargetASC, FGameplayEffectSpecHandle DamageEffectHandle)
{
	if (UGameplayAbility* CurrentAbility = RsAbilitySystemComponent->GetAnimatingAbility())
	{
		PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(CurrentAbility, Duration);
		PauseMontageTask->ReadyForActivation();
	}
}
