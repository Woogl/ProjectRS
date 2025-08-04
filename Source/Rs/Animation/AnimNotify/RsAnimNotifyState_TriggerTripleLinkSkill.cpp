// Copyright 2025 Team BH.


#include "RsAnimNotifyState_TriggerTripleLinkSkill.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_WaitDamageEffectApplied.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_WaitDamageEffectBlockedImmunity.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/Character/RsEnemyCharacter.h"

URsAnimNotifyState_TriggerTripleLinkSkill::URsAnimNotifyState_TriggerTripleLinkSkill()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotifyState_TriggerTripleLinkSkill::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (CurrentAbility.IsValid())
	{
		// Handle damage received.
		FGameplayTagRequirements Requirements;
		WaitAppliedTask = URsAbilityTask_WaitDamageEffectApplied::WaitDamageEffect(CurrentAbility.Get(), ReceivedDamageTags, false, true);
		WaitAppliedTask->OnApplied.AddDynamic(this, &ThisClass::HandleReceiveDamage);
		WaitAppliedTask->ReadyForActivation();

		// Damage can be blocked by other immnune effects.
		if (bTriggerOnDamageImmunity == true)
		{
			WaitBlockedTask = URsAbilityTask_WaitDamageEffectBlockedImmunity::WaitDamageEffectBlockedByImmunity(CurrentAbility.Get(),ReceivedDamageTags, false, true);
			WaitBlockedTask->Blocked.AddDynamic(this, &ThisClass::HandleBlockDamage);
			WaitBlockedTask->ReadyForActivation();
		}
	}
}

void URsAnimNotifyState_TriggerTripleLinkSkill::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (WaitAppliedTask.IsValid())
	{
		WaitAppliedTask->EndTask();
	}

	if (WaitBlockedTask.IsValid())
	{
		WaitBlockedTask->EndTask();
	}
}

void URsAnimNotifyState_TriggerTripleLinkSkill::HandleReceiveDamage(AActor* Source, FGameplayEffectSpecHandle SpecHandle, FActiveGameplayEffectHandle ActiveHandle)
{
	if (!OwnerASC.IsValid())
	{
		return;
	}
	
	AActor* Avatar = OwnerASC->GetAvatarActor();
	if (URsBattleSubsystem* BattleSubsystem = Avatar->GetWorld()->GetSubsystem<URsBattleSubsystem>())
	{
		if (ARsEnemyCharacter* Enemy = Cast<ARsEnemyCharacter>(Avatar))
		{
			BattleSubsystem->SetLinkSkillTarget(Enemy, ERsLinkSkillType::Triple, 3);
		}
	}
}

void URsAnimNotifyState_TriggerTripleLinkSkill::HandleBlockDamage(FGameplayEffectSpecHandle BlockedSpec, FActiveGameplayEffectHandle ImmunityGameplayEffectHandle)
{
	if (!OwnerASC.IsValid())
	{
		return;
	}

	AActor* Avatar = OwnerASC->GetAvatarActor();
	if (URsBattleSubsystem* BattleSubsystem = Avatar->GetWorld()->GetSubsystem<URsBattleSubsystem>())
	{
		if (ARsEnemyCharacter* Enemy = Cast<ARsEnemyCharacter>(Avatar))
		{
			BattleSubsystem->SetLinkSkillTarget(Enemy, ERsLinkSkillType::Triple, 3);
		}
	}
}
