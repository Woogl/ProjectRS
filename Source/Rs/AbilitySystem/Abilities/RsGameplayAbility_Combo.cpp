// Copyright 2024 Team BH.


#include "RsGameplayAbility_Combo.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

URsGameplayAbility_Combo::URsGameplayAbility_Combo()
{
}

void URsGameplayAbility_Combo::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	for (TSubclassOf<URsGameplayAbility> InnerAbility : InnerAbilities)
	{
		FGameplayAbilitySpec InnerSpec(InnerAbility, GetAbilityLevel(), INDEX_NONE, ActorInfo->OwnerActor.Get());
		InnerSpecs.Add(InnerSpec);
		
		FGameplayAbilitySpecHandle InnerSpecHandle = ActorInfo->AbilitySystemComponent->GiveAbility(InnerSpec);
		InnerHandles.Add(InnerSpecHandle);
	}
	
	MaxComboIndex = InnerAbilities.Num();
	
	ActorInfo->AbilitySystemComponent->AbilityActivatedCallbacks.AddUObject(this, &ThisClass::HandleInnerAbilityActivated);
	ActorInfo->AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &ThisClass::HandleInnerAbilityEnded);
}

void URsGameplayAbility_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ComboWindowStartTag.IsValid())
	{
		UAbilityTask_WaitGameplayEvent* ComboWindowStartTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ComboWindowStartTag);
		ComboWindowStartTask->EventReceived.AddDynamic(this, &ThisClass::HandleComboWindowStarted);
		ComboWindowStartTask->ReadyForActivation();
	}
	
	if (ComboWindowEndTag.IsValid())
	{
		UAbilityTask_WaitGameplayEvent* ComboWindowEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ComboWindowEndTag);
		ComboWindowEndTask->EventReceived.AddDynamic(this, &ThisClass::HandleComboWindowEnded);
		ComboWindowEndTask->ReadyForActivation();
	}
	
	ActivateInnerAbility();
}

void URsGameplayAbility_Combo::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ResetComboIndex();
}

void URsGameplayAbility_Combo::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	ActorInfo->AbilitySystemComponent->OnAbilityEnded.RemoveAll(this);
	
	for (const FGameplayAbilitySpecHandle& InnerSpecHandle : InnerHandles)
	{
		ActorInfo->AbilitySystemComponent->ClearAbility(InnerSpecHandle);
	}
	
	Super::OnRemoveAbility(ActorInfo, Spec);
}

void URsGameplayAbility_Combo::ActivateInnerAbility()
{
	if (GetAbilitySystemComponentFromActorInfo()->TryActivateAbility(InnerHandles[CurrentComboIndex]))
	{
		IncrementComboIndex();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

int32 URsGameplayAbility_Combo::IncrementComboIndex()
{
	CurrentComboIndex++;
	if (CurrentComboIndex >= MaxComboIndex)
	{
		CurrentComboIndex = 0;
	}
	return CurrentComboIndex;
}

void URsGameplayAbility_Combo::ResetComboIndex()
{
	CurrentComboIndex = 0;
}

void URsGameplayAbility_Combo::HandleComboWindowStarted(FGameplayEventData Data)
{
	InputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
	InputPressTask->OnPress.AddDynamic(this, &ThisClass::HandleInputPressed);
	InputPressTask->ReadyForActivation();
}

void URsGameplayAbility_Combo::HandleComboWindowEnded(FGameplayEventData Data)
{
	if (InputPressTask)
	{
		InputPressTask->EndTask();
		InputPressTask = nullptr;
	}
}

void URsGameplayAbility_Combo::HandleInputPressed(float TimeWaited)
{
	if (InputPressTask)
	{
		InputPressTask->EndTask();
		InputPressTask = nullptr;
	}
	
	ActivateInnerAbility();
}

void URsGameplayAbility_Combo::HandleInnerAbilityActivated(UGameplayAbility* ActivatedAbility)
{
	FGameplayAbilitySpecHandle ActivatedHandle = ActivatedAbility->GetCurrentAbilitySpecHandle();
	if (InnerHandles.Contains(ActivatedHandle))
	{
		InnerHandlesActivating.Add(ActivatedHandle);
	}
}

void URsGameplayAbility_Combo::HandleInnerAbilityEnded(const FAbilityEndedData& AbilityEndData)
{
	if (InnerHandles.Contains(AbilityEndData.AbilitySpecHandle))
	{
		InnerHandlesActivating.Remove(AbilityEndData.AbilitySpecHandle);
		if (InnerHandlesActivating.IsEmpty() == true)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, AbilityEndData.bWasCancelled);
		}
	}
}
