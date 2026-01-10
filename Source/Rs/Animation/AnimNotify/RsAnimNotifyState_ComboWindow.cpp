// Copyright 2025 Team BH.


#include "RsAnimNotifyState_ComboWindow.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_WaitEnhancedInput.h"

void URsAnimNotifyState_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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
	UGameplayAbility* CurrentAbility = ASC->GetAnimatingAbility();
	if (!CurrentAbility)
	{
		return;
	}
	
	if (FComboWindowRuntimeData* Existing = RuntimeDataMap.Find(MeshComp))
	{
		for (TWeakObjectPtr<URsAbilityTask_WaitEnhancedInput> WeakTask : Existing->Tasks)
		{
			if (URsAbilityTask_WaitEnhancedInput* Task = WeakTask.Get())
			{
				Task->EndTask();
			}
		}
		RuntimeDataMap.Remove(MeshComp);
	}
	
	FComboWindowRuntimeData Data;
	
	for (const FGameplayTagContainer& AbilityTags : AbilityTagsList)
	{
		URsGameplayAbility* Ability = URsAbilitySystemLibrary::FindRsAbilityWithTags(ASC, AbilityTags, true);
		if (!Ability)
		{
			continue;
		}
		TObjectPtr<UInputAction> InputAction = Ability->ActivationInputAction;
		if (!InputAction)
		{
			continue;
		}

		URsAbilityTask_WaitEnhancedInput* Task = URsAbilityTask_WaitEnhancedInput::WaitEnhancedInput(CurrentAbility, NAME_None, InputAction, true);
		const FGameplayTagContainer TagsCopy = AbilityTags;
		const TWeakObjectPtr<USkeletalMeshComponent> WeakMesh = MeshComp;
		Task->InputEventReceivedNative.AddWeakLambda(this, [this, WeakMesh, TagsCopy](const FInputActionValue& Value, const APawn* Pawn)
		{
			if (USkeletalMeshComponent* MeshComp = WeakMesh.Get())
			{
				HandleInputTriggered(Value, MeshComp, TagsCopy);
			}
		});
		
		Task->ReadyForActivation();
		
		Data.Tasks.Add(Task);
	}
	
	RuntimeDataMap.Add(MeshComp, Data);
}

void URsAnimNotifyState_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (FComboWindowRuntimeData* Data = RuntimeDataMap.Find(MeshComp))
	{
		for (TWeakObjectPtr<URsAbilityTask_WaitEnhancedInput> WeakTask : Data->Tasks)
		{
			if (URsAbilityTask_WaitEnhancedInput* Task = WeakTask.Get())
			{
				Task->EndTask();
			}
		}
	}
	RuntimeDataMap.Remove(MeshComp);
}

void URsAnimNotifyState_ComboWindow::HandleInputTriggered(const FInputActionValue& Value, USkeletalMeshComponent* MeshComp, const FGameplayTagContainer& AbilityTags)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner || !PassCondition(Owner))
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (!ASC)
	{
		return;
	}
	
	if (bCancelCurrentAbility)
	{
		if (UGameplayAbility* CurrentAbility = ASC->GetAnimatingAbility())
		{
			CurrentAbility->K2_CancelAbility();
		}
	}

	ASC->TryActivateAbilitiesByTag(AbilityTags);
	
	if (FComboWindowRuntimeData* Data = RuntimeDataMap.Find(MeshComp))
	{
		for (TWeakObjectPtr<URsAbilityTask_WaitEnhancedInput> WeakTask : Data->Tasks)
		{
			if (URsAbilityTask_WaitEnhancedInput* Task = WeakTask.Get())
			{
				Task->EndTask();
			}
		}
	}
	RuntimeDataMap.Remove(MeshComp);
}
