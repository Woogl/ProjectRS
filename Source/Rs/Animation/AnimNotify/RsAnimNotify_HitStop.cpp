// Copyright 2024 Team BH.


#include "RsAnimNotify_HitStop.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_HitStop.h"
#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"

void URsAnimNotify_HitStop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

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

#if WITH_EDITOR
	UWorld* World = MeshComp->GetWorld();
	if (Duration > 0.f && bEnableInEditorPreview && World->WorldType == EWorldType::EditorPreview)
	{
		CachedMeshComp = MeshComp;
		CachedMeshComp->Stop();
		World->GetTimerManager().SetTimer(Timer, this, &ThisClass::ResumePlay, Duration, false);
	}
#endif // WITH_EDITOR
}

void URsAnimNotify_HitStop::HandleDealDamage(UAbilitySystemComponent* TargetASC, FGameplayEffectSpecHandle DamageEffectHandle)
{
	if (UGameplayAbility* CurrentAbility = RsAbilitySystemComponent->GetAnimatingAbility())
	{
		URsAbilityTask_HitStop* HitStopTask = URsAbilityTask_HitStop::StartHitStop(CurrentAbility, Duration);
		HitStopTask->ReadyForActivation();
	}
}

#if WITH_EDITOR
void URsAnimNotify_HitStop::ResumePlay()
{
	CachedMeshComp->Play(true);
}
#endif // WITH_EDITOR
