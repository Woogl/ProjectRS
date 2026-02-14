// Copyright 2025 Team BH.


#include "RsAnimNotifyState_Guard.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void URsAnimNotifyState_Guard::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	if (!PassCondition(Owner))
	{
		return;
	}
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (!ASC)
	{
		return;
	}
	UGameplayAbility* Ability = ASC->GetAnimatingAbility();
	if (!Ability)
	{
		return;
	}

	if (GuardEffect)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		ASC->BP_ApplyGameplayEffectToSelf(GuardEffect, 0, EffectContext);
	}
}

void URsAnimNotifyState_Guard::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (!ASC)
	{
		return;
	}
	
	if (GuardEffect)
	{
		FGameplayEffectQuery Query;
		Query.EffectDefinition = GuardEffect;
		ASC->RemoveActiveEffects(Query);
	}
}
