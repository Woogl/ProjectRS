// Copyright 2025 Team BH.


#include "RsAnimNotifyState_PerfectGuard.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectBlockedImmunity.h"
#include "Rs/RsGameplayTags.h"

URsAnimNotifyState_PerfectGuard::URsAnimNotifyState_PerfectGuard()
{
	DamageTags.AddTag(RsGameplayTags::EFFECT_DAMAGE);
}

void URsAnimNotifyState_PerfectGuard::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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

	if (InvincibleEffect)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		ASC->BP_ApplyGameplayEffectToSelf(InvincibleEffect, 0, EffectContext);
	}

	UAbilityTask_WaitGameplayEffectBlockedImmunity* BlockTask = UAbilityTask_WaitGameplayEffectBlockedImmunity::WaitGameplayEffectBlockedByImmunity(Ability, FGameplayTagRequirements(), FGameplayTagRequirements());
	if (!BlockTask)
	{
		return;
	}
	
	BlockTask->Blocked.AddDynamic(this, &ThisClass::HandleDamageBlocked);
	BlockTask->ReadyForActivation();
}

void URsAnimNotifyState_PerfectGuard::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
	
	if (InvincibleEffect)
	{
		FGameplayEffectQuery Query;
		Query.EffectDefinition = InvincibleEffect;
		ASC->RemoveActiveEffects(Query);
	}
}

void URsAnimNotifyState_PerfectGuard::HandleDamageBlocked(FGameplayEffectSpecHandle BlockedSpec, FActiveGameplayEffectHandle ImmunityGameplayEffectHandle)
{
	if (!CounterEffect)
	{
		return;
	}
	
	if (!BlockedSpec.IsValid())
	{
		return;
	}
	
	FGameplayTagContainer OutTags;
	BlockedSpec.Data->GetAllAssetTags(OutTags);
	if (!OutTags.HasAny(DamageTags))
	{
		return;
	}

	UAbilitySystemComponent* AttackerASC = BlockedSpec.Data->GetEffectContext().GetInstigatorAbilitySystemComponent();
	UAbilitySystemComponent* DefenderASC = ImmunityGameplayEffectHandle.GetOwningAbilitySystemComponent();
	if (!AttackerASC || !DefenderASC)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = ImmunityGameplayEffectHandle.GetOwningAbilitySystemComponent()->MakeEffectContext();
	AttackerASC->BP_ApplyGameplayEffectToSelf(CounterEffect, 0, EffectContext);
	
	DefenderASC->RemoveLooseGameplayTag(RsGameplayTags::COOLDOWN_GUARD);
}
