// Copyright 2025 Team BH.


#include "RsAnimNotifyState_PreHit.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

// AnimNotify_GameplayCue.cpp
typedef void (*GameplayCueFunc)(AActor* Target, const FGameplayTag GameplayCueTag, const FGameplayCueParameters& Parameters);

static void ProcessGameplayCue(GameplayCueFunc Func, USkeletalMeshComponent* MeshComp, FGameplayTag GameplayCue, UAnimSequenceBase* Animation)
{
	if (MeshComp && GameplayCue.IsValid())
	{
		AActor* OwnerActor = MeshComp->GetOwner();

#if WITH_EDITOR
		if (GIsEditor && (OwnerActor == nullptr))
		{
			// Make preview work in anim preview window
			UGameplayCueManager::PreviewComponent = MeshComp;
			UGameplayCueManager::PreviewWorld = MeshComp->GetWorld();

			if (UGameplayCueManager* GCM = UAbilitySystemGlobals::Get().GetGameplayCueManager())
			{
				GCM->LoadNotifyForEditorPreview(GameplayCue);
			}
		}
#endif // #if WITH_EDITOR

		FGameplayCueParameters Parameters;
		Parameters.Instigator = OwnerActor;

		// Try to get the ability level. This may not be able to find the ability level
		// in cases where a blend out is happening or two abilities are trying to play animations
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor))
		{
			if (ASC->GetCurrentMontage() == Animation)
			{
				if (const UGameplayAbility* Ability = ASC->GetAnimatingAbility())
				{
					Parameters.AbilityLevel = Ability->GetAbilityLevel();
				}
			}

			// Always use ASC's owner for instigator
			Parameters.Instigator = ASC->GetOwner();
		}

		Parameters.TargetAttachComponent = MeshComp;

		(*Func)(OwnerActor, GameplayCue, Parameters);
	}

#if WITH_EDITOR
	if (GIsEditor)
	{
		UGameplayCueManager::PreviewComponent = nullptr;
		UGameplayCueManager::PreviewWorld = nullptr;
	}
#endif // #if WITH_EDITOR
}
// ~AnimNotify_GameplayCue.cpp

URsAnimNotifyState_PreHit::URsAnimNotifyState_PreHit()
{
}

void URsAnimNotifyState_PreHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!PassCondition(Owner))
	{
		return;
	}
	
	FPreHitRuntimeData& Data = RuntimeDataMap.Add(MeshComp);
	
	TArray<AActor*> OutTargets;
	if (URsTargetingLibrary::PerformTargetingFromComponent(MeshComp, TargetingParams, OutTargets))
	{
		for (AActor* Target : OutTargets)
		{
			Data.Targets.Add(Target);
			if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target))
			{
				ASC->AddLooseGameplayTag(GrantTargetTag);
			}
		}
	}

	// Runtime
	if (!OutTargets.IsEmpty())
	{
		ProcessGameplayCue(&UGameplayCueManager::ExecuteGameplayCue_NonReplicated, MeshComp, GameplayCue.GameplayCueTag, Animation);
	}
	
#if WITH_EDITOR
	// Editor preview
	UWorld* World = MeshComp->GetWorld();
	if (World && World->WorldType == EWorldType::EditorPreview)
	{
		ProcessGameplayCue(&UGameplayCueManager::ExecuteGameplayCue_NonReplicated, MeshComp, GameplayCue.GameplayCueTag, Animation);
	}
#endif // WITH_EDITOR
}

void URsAnimNotifyState_PreHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (FPreHitRuntimeData* Data = RuntimeDataMap.Find(MeshComp))
	{
		for (TWeakObjectPtr<AActor> WeakTarget : Data->Targets)
		{
			if (AActor* Target = WeakTarget.Get())
			{
				if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target))
				{
					ASC->RemoveLooseGameplayTag(GrantTargetTag);
				}
			}
		}
	}
	
	RuntimeDataMap.Remove(MeshComp);
}
