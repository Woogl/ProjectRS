// Copyright 2024 Team BH.


#include "RsAnimNotifyState_HitStop.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_PauseMontage.h"

void URsAnimNotifyState_HitStop::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (UAbilitySystemComponent* ASC = OwnerASC.Get())
	{
		FGameplayTag HitScanTag = FGameplayTag::RequestGameplayTag(TEXT("AnimNotify.HitScan"));
		FGameplayTag HitTraceTag = FGameplayTag::RequestGameplayTag(TEXT("AnimNotify.HitTrace"));
		
		ASC->GenericGameplayEventCallbacks.FindOrAdd(HitScanTag).AddUObject(this, &ThisClass::HandleAttackHit);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(HitTraceTag).AddUObject(this, &ThisClass::HandleAttackHit);
	}
}

void URsAnimNotifyState_HitStop::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (UAbilitySystemComponent* ASC = OwnerASC.Get())
	{
		FGameplayTag HitScanTag = FGameplayTag::RequestGameplayTag(TEXT("AnimNotify.HitScan"));
		FGameplayTag HitTraceTag = FGameplayTag::RequestGameplayTag(TEXT("AnimNotify.HitTrace"));
		
		ASC->GenericGameplayEventCallbacks.FindOrAdd(HitScanTag).RemoveAll(this);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(HitTraceTag).RemoveAll(this);
	}
}

void URsAnimNotifyState_HitStop::HandleAttackHit(const FGameplayEventData* EventData)
{
	if (CurrentAbility.IsValid())
	{
		PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(CurrentAbility.Get(), Duration);
		PauseMontageTask->ReadyForActivation();
	}
}
