// Copyright 2024 Team BH.


#include "RsAnimNotify_HitStop.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_HitStop.h"

void URsAnimNotify_HitStop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			if (UGameplayAbility* CurrentAbility = ASC->GetAnimatingAbility())
			{
				URsAbilityTask_HitStop* HitStopTask = URsAbilityTask_HitStop::StartHitStop(CurrentAbility, Duration);
				HitStopTask->ReadyForActivation();
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

#if WITH_EDITOR
void URsAnimNotify_HitStop::ResumePlay()
{
	CachedMeshComp->Play(true);
}
#endif // WITH_EDITOR
