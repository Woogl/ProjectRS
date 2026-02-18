// Copyright 2025 Team BH.


#include "RsAnimNotifyState_PerfectGuard.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectApplied_Self.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectBlockedImmunity.h"
#include "Rs/RsGameplayTags.h"

URsAnimNotifyState_PerfectGuard::URsAnimNotifyState_PerfectGuard()
{
	DamageTags.AddTag(RsGameplayTags::EFFECT_DAMAGE);
	WarningDamageTag = RsGameplayTags::EFFECT_DAMAGE_GUARDPIERCE;
}

void URsAnimNotifyState_PerfectGuard::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
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
	UGameplayAbility* Ability = ASC->GetAnimatingAbility();
	if (!Ability)
	{
		return;
	}

	FActiveGameplayEffectHandle InvincibleHandle;
	if (GuardEffect)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		InvincibleHandle = ASC->BP_ApplyGameplayEffectToSelf(GuardEffect, 0, EffectContext);
	}
	
	FGameplayTargetDataFilterHandle Filter;
	FGameplayTagRequirements ActorTagReqs;
	FGameplayTagRequirements DamageTagReqs;
	DamageTagReqs.RequireTags = DamageTags;
	UAbilityTask_WaitGameplayEffectApplied_Self* AppliedTask = UAbilityTask_WaitGameplayEffectApplied_Self::WaitGameplayEffectAppliedToSelf(Ability, Filter, ActorTagReqs, ActorTagReqs, DamageTagReqs, FGameplayTagRequirements());
	if (!AppliedTask)
	{
		return;
	}
	AppliedTask->OnApplied.AddDynamic(this, &ThisClass::HandleDamageApplied);
	AppliedTask->ReadyForActivation();
	
	if (UWorld* World = MeshComp->GetWorld())
	{
		FTimerHandle TimerHandle;
		TWeakObjectPtr<UAbilitySystemComponent> WeakASC = ASC;
		TWeakObjectPtr<UAbilityTask_WaitGameplayEffectApplied_Self> WeakTask = AppliedTask;
		World->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([WeakTask, WeakASC, InvincibleHandle]()
		{
			if (WeakTask.IsValid())
			{
				WeakTask->EndTask();
			}
			if (WeakASC.IsValid())
			{
				WeakASC->RemoveActiveGameplayEffect(InvincibleHandle);
			}
		}),
	TotalDuration, false);
	}
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
	
	if (GuardEffect)
	{
		FGameplayEffectQuery Query;
		Query.EffectDefinition = GuardEffect;
		ASC->RemoveActiveEffects(Query);
	}
}

void URsAnimNotifyState_PerfectGuard::HandleDamageApplied(AActor* Source, FGameplayEffectSpecHandle SpecHandle, FActiveGameplayEffectHandle ActiveHandle)
{
	if (!SpecHandle.IsValid())
	{
		return;
	}
	
	FGameplayEffectContextHandle DamageContext = SpecHandle.Data->GetEffectContext();
	if (!DamageContext.IsValid())
	{
		return;
	}
	
	UAbilitySystemComponent* AttackerASC = DamageContext.GetInstigatorAbilitySystemComponent();
	UAbilitySystemComponent* DefenderASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(DamageContext.GetHitResult()->GetActor());
	if (!AttackerASC || !DefenderASC)
	{
		return;
	}

	if (CounterEffect)
	{
		FGameplayEffectContextHandle CounterContext = DefenderASC->MakeEffectContext();
		DefenderASC->ApplyGameplayEffectToTarget(CounterEffect->GetDefaultObject<UGameplayEffect>(), AttackerASC, 0, CounterContext);
		
		FGameplayTagContainer OutTags;
		SpecHandle.Data->GetAllAssetTags(OutTags);
		if (OutTags.HasTagExact(WarningDamageTag))
		{
			FGameplayEventData Payload;
			Payload.EventTag = WarningCounterHitReaction;
			Payload.ContextHandle = CounterContext;
			Payload.Instigator = DefenderASC->GetAvatarActor();
			AttackerASC->HandleGameplayEvent(WarningCounterHitReaction, &Payload);
		}
	}

	if (DefenderASC->HasMatchingGameplayTag(RsGameplayTags::COOLDOWN_GUARD))
	{
		DefenderASC->RemoveLooseGameplayTag(RsGameplayTags::COOLDOWN_GUARD);
	}
}
