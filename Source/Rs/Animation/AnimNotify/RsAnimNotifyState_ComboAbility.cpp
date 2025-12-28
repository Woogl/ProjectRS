// Copyright 2025 Team BH.


#include "RsAnimNotifyState_ComboAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
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
		UGameplayAbility* CurrentAbility = ASC->GetAnimatingAbility();
		URsGameplayAbility* NewAbility = URsAbilitySystemLibrary::FindRsAbilityWithTags(ASC, AbilityTags, true);
		if (CurrentAbility && NewAbility)
		{
			if (TObjectPtr<UInputAction> InputAction = NewAbility->ActivationInputAction)
			{
				Task = URsAbilityTask_WaitEnhancedInput::WaitEnhancedInput(CurrentAbility, NAME_None, InputAction, true);
				Task->InputEventReceived.AddDynamic(this, &ThisClass::HandleInputPressed);
				Task->ReadyForActivation();
			}
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

void URsAnimNotifyState_ComboAbility::HandleInputPressed(const FInputActionValue& Value, const APawn* Pawn)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn))
	{
		if (bCancelCurrentAbility)
		{
			if (UGameplayAbility* CurrentAbility = ASC->GetAnimatingAbility())
			{
				CurrentAbility->K2_CancelAbility();
			}
		}
		ASC->TryActivateAbilitiesByTag(AbilityTags);
	}
}
