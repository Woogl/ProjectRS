// Copyright 2025 Team BH.


#include "RsAnimNotifyState_ComboAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_WaitEnhancedInput.h"

void URsAnimNotifyState_ComboAbility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	if (!PassCondition(Owner))
	{
		return;
	}
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
	{
		if (UGameplayAbility* CurrentAbility = ASC->GetAnimatingAbility())
		{
			Task = URsAbilityTask_WaitEnhancedInput::WaitEnhancedInput(CurrentAbility, NAME_None, InputAction, true);
			const FGameplayTagContainer TagsCopy = AbilityTags;
			const TWeakObjectPtr<USkeletalMeshComponent> WeakMesh = MeshComp;
			Task->InputEventReceivedNative.AddWeakLambda(this, [this, WeakMesh, TagsCopy](const FInputActionValue& Value, const APawn* Pawn)
			{
				if (USkeletalMeshComponent* MeshComp = WeakMesh.Get())
				{
					HandleInputPressed(Value, WeakMesh.Get(), TagsCopy);
				}
			});
			Task->ReadyForActivation();
		}
	}
}

void URsAnimNotifyState_ComboAbility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (Task.IsValid())
	{
		Task->EndTask();
	}
}

void URsAnimNotifyState_ComboAbility::HandleInputPressed(const FInputActionValue& Value, USkeletalMeshComponent* MeshComp, const FGameplayTagContainer& InAbilityTags)
{
	AActor* Owner = MeshComp->GetOwner();
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
	{
		if (bCancelCurrentAbility)
		{
			if (UGameplayAbility* CurrentAbility = ASC->GetAnimatingAbility())
			{
				CurrentAbility->K2_CancelAbility();
			}
		}
		ASC->TryActivateAbilitiesByTag(InAbilityTags);
	}
}
