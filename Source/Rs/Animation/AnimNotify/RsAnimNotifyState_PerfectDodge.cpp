// Copyright 2025 Team BH.


#include "RsAnimNotifyState_PerfectDodge.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_WaitDamageEffectBlockedImmunity.h"
#include "Rs/Character/RsPlayerCharacter.h"

URsAnimNotifyState_PerfectDodge::URsAnimNotifyState_PerfectDodge()
{
	DodgeCounterAbilityTags.AddTag(RsGameplayTags::ABILITY_DODGE_PERFECT);
	
	DamageTags.AddTag(RsGameplayTags::EFFECT_DAMAGE);
}

void URsAnimNotifyState_PerfectDodge::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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

	URsAbilityTask_WaitDamageEffectBlockedImmunity* DamageBlockTask = URsAbilityTask_WaitDamageEffectBlockedImmunity::WaitDamageEffectBlockedByImmunity(Ability, DamageTags, true);
	DamageBlockTask->Blocked.AddUniqueDynamic(this, &ThisClass::HandleDamageBlocked);
	DamageBlockTask->ReadyForActivation();

	FDodgeInvincibleRuntimeData Data;
	
	if (InvincibleEffect)
	{
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		FActiveGameplayEffectHandle InvincibleEffectHandle = ASC->BP_ApplyGameplayEffectToSelf(InvincibleEffect, 0.f, Context);
		
		Data.DamageBlockTask = DamageBlockTask;
		Data.InvincibleEffectHandle = InvincibleEffectHandle;
	}
	
	if (ARsPlayerCharacter* PlayerCharacter = Cast<ARsPlayerCharacter>(MeshComp->GetOwner()))
	{
		PlayerCharacter->EnableJustDodgeCapsule(bEnableJustDodgeCapsule);
	}
	
	RuntimeDataMap.Add(MeshComp, Data);
}

void URsAnimNotifyState_PerfectDodge::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	
	if (FDodgeInvincibleRuntimeData* Data = RuntimeDataMap.Find(MeshComp))
	{
		if (URsAbilityTask_WaitDamageEffectBlockedImmunity* Task = Data->DamageBlockTask.Get())
		{
			Task->EndTask();
		}
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			ASC->RemoveActiveGameplayEffect(Data->InvincibleEffectHandle);
		}
	}
	if (ARsPlayerCharacter* PlayerCharacter = Cast<ARsPlayerCharacter>(Owner))
	{
		PlayerCharacter->EnableJustDodgeCapsule(false);
	}
	
	RuntimeDataMap.Remove(MeshComp);
}

void URsAnimNotifyState_PerfectDodge::HandleDamageBlocked(FGameplayEffectSpecHandle BlockedSpec, FActiveGameplayEffectHandle ImmunityGameplayEffectHandle)
{
	if (UAbilitySystemComponent* ASC = ImmunityGameplayEffectHandle.GetOwningAbilitySystemComponent())
	{
		ASC->TryActivateAbilitiesByTag(DodgeCounterAbilityTags);
	}
}
