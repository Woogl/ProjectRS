// Copyright 2025 Team BH.


#include "RsAnimNotifyState_DodgeInvincible.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_WaitDamageEffectBlockedImmunity.h"
#include "Rs/Character/RsPlayerCharacter.h"

URsAnimNotifyState_DodgeInvincible::URsAnimNotifyState_DodgeInvincible()
{
	JustDodgeAbilityTags.AddTag(RsGameplayTags::ABILITY_DODGE);
	JustDodgeAbilityTags.AddTag(RsGameplayTags::ABILITY_JUST);
	
	DamageTags.AddTag(RsGameplayTags::EFFECT_DAMAGE);
}

void URsAnimNotifyState_DodgeInvincible::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (!ASC.IsValid())
	{
		return;
	}
	UGameplayAbility* Ability = ASC->GetAnimatingAbility();
	if (!Ability)
	{
		return;
	}
	
	DamageBlockTask = URsAbilityTask_WaitDamageEffectBlockedImmunity::WaitDamageEffectBlockedByImmunity(Ability, DamageTags, true);
	DamageBlockTask->Blocked.AddUniqueDynamic(this, &ThisClass::HandleDamageBlocked);
	DamageBlockTask->ReadyForActivation();
	
	if (InvincibleEffect)
	{
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(InvincibleEffect, 0.f, ASC->MakeEffectContext());
		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(RsGameplayTags::SETBYCALLER_MAGNITUDE, InvincibleTier);
			InvincibleEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
	
	if (ARsPlayerCharacter* PlayerCharacter = Cast<ARsPlayerCharacter>(ASC->GetAvatarActor()))
	{
		PlayerCharacter->EnableJustDodgeCapsule(true);
	}
}

void URsAnimNotifyState_DodgeInvincible::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (DamageBlockTask.IsValid())
	{
		DamageBlockTask->EndTask();
		DamageBlockTask.Reset();
	}

	if (!ASC.IsValid())
	{
		return;
	}
	
	if (InvincibleEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(InvincibleEffectHandle);
	}

	if (ARsPlayerCharacter* PlayerCharacter = Cast<ARsPlayerCharacter>(ASC->GetAvatarActor()))
	{
		PlayerCharacter->EnableJustDodgeCapsule(false);
	}
}

void URsAnimNotifyState_DodgeInvincible::HandleDamageBlocked(FGameplayEffectSpecHandle BlockedSpec, FActiveGameplayEffectHandle ImmunityGameplayEffectHandle)
{
	if (!ASC.IsValid())
	{
		return;
	}
	
	ASC->TryActivateAbilitiesByTag(JustDodgeAbilityTags);
}
