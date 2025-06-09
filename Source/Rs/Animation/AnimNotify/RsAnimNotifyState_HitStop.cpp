// Copyright 2024 Team BH.


#include "RsAnimNotifyState_HitStop.h"

#include "Rs/AbilitySystem/Abilities/RsGameplayAbility_Attack.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_PauseMontage.h"

void URsAnimNotifyState_HitStop::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// TODO: Integrate ability
	if (URsGameplayAbility_Attack* AttackAbility = Cast<URsGameplayAbility_Attack>(CurrentAbility))
	{
		AttackAbility->OnAttackHitTarget.AddDynamic(this, &ThisClass::HandleAttackHit);
	}
}

void URsAnimNotifyState_HitStop::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	// TODO: Integrate ability
	if (URsGameplayAbility_Attack* AttackAbility = Cast<URsGameplayAbility_Attack>(CurrentAbility))
	{
		AttackAbility->OnAttackHitTarget.RemoveDynamic(this, &ThisClass::HandleAttackHit);
	}
}

void URsAnimNotifyState_HitStop::HandleAttackHit(const AActor* Target, const FGameplayTag& DamageEvent)
{
	if (CurrentAbility.IsValid())
	{
		PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(CurrentAbility.Get(), Duration);
		PauseMontageTask->ReadyForActivation();
	}
}
