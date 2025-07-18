// Copyright 2024 Team BH.


#include "RsAnimNotify_PauseMontage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_PauseMontage.h"

URsAnimNotify_PauseMontage::URsAnimNotify_PauseMontage()
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = true;
#endif // WITH_EDITORONLY_DATA
}

void URsAnimNotify_PauseMontage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			if (UGameplayAbility* CurrentAbility = ASC->GetAnimatingAbility())
			{
				URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(CurrentAbility, Duration);
				PauseMontageTask->ReadyForActivation();
			}
		}
	}

#if WITH_EDITOR
	UWorld* World = MeshComp->GetWorld();
	if (Duration > 0.f && World->WorldType == EWorldType::EditorPreview)
	{
		CachedMeshComp = MeshComp;
		CachedMeshComp->Stop();
		World->GetTimerManager().SetTimer(Timer, this, &ThisClass::ResumeMontage, Duration, false);
	}
#endif // WITH_EDITOR
}

#if WITH_EDITOR
void URsAnimNotify_PauseMontage::ResumeMontage()
{
	CachedMeshComp->Play(true);
}
#endif // WITH_EDITOR
