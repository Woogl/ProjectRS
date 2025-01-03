// Copyright 2024 Team BH.


#include "RsGameplayAbility_Combo.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

URsGameplayAbility_Combo::URsGameplayAbility_Combo()
{
	bRetriggerInstancedAbility = true;
}

void URsGameplayAbility_Combo::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (ComboWindowStartTag.IsValid())
	{
		if (UAbilityTask_WaitGameplayEvent* ComboWindowStartTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ComboWindowStartTag))
		{
			ComboWindowStartTask->EventReceived.AddDynamic(this, &ThisClass::HandleComboWindowStarted);
			ComboWindowStartTask->Activate();
		}
	}

	if (ComboWindowEndTag.IsValid())
	{
		if (UAbilityTask_WaitGameplayEvent* ComboWindowEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ComboWindowEndTag))
		{
			ComboWindowEndTask->EventReceived.AddDynamic(this, &ThisClass::HandleComboWindowEnded);
			ComboWindowEndTask->Activate();
		}
	}
	
	ActorInfo->AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &ThisClass::HandleInnerAbilityEnded);

	for (TSubclassOf<URsGameplayAbility> InnerAbility : InnerAbilities)
	{
		FGameplayAbilitySpec InnerSpec(InnerAbility, GetAbilityLevel(), INDEX_NONE, ActorInfo->AvatarActor.Get());
		InnerSpecs.Add(InnerSpec);
		
		FGameplayAbilitySpecHandle InnerSpecHandle = ActorInfo->AbilitySystemComponent->GiveAbility(InnerSpec);
		InnerSpecHandles.Add(InnerSpecHandle);
	}
	
	MaxComboIndex = InnerAbilities.Num();
}

void URsGameplayAbility_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ActorInfo->AbilitySystemComponent->TryActivateAbility(InnerSpecHandles[CurrentComboIndex]);
	IncreaseComboIndex();

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URsGameplayAbility_Combo::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	ActorInfo->AbilitySystemComponent->OnAbilityEnded.RemoveAll(this);
	
	for (const FGameplayAbilitySpecHandle& InnerSpecHandle : InnerSpecHandles)
	{
		ActorInfo->AbilitySystemComponent->ClearAbility(InnerSpecHandle);
	}
	
	Super::OnRemoveAbility(ActorInfo, Spec);
}

int32 URsGameplayAbility_Combo::IncreaseComboIndex()
{
	CurrentComboIndex++;
	if (CurrentComboIndex >= MaxComboIndex)
	{
		CurrentComboIndex = 0;
	}
	return CurrentComboIndex;
}

void URsGameplayAbility_Combo::HandleComboWindowStarted(FGameplayEventData Data)
{
	CachedPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
	CachedPressTask->OnPress.AddDynamic(this, &ThisClass::HandleInputPressed);
	CachedPressTask->Activate();
}

void URsGameplayAbility_Combo::HandleComboWindowEnded(FGameplayEventData Data)
{
	if (CachedPressTask)
	{
		CachedPressTask->EndTask();
		CachedPressTask = nullptr;
	}
}

void URsGameplayAbility_Combo::HandleInputPressed(float TimeWaited)
{
	GetAbilitySystemComponentFromActorInfo()->TryActivateAbility(InnerSpecHandles[CurrentComboIndex]);
	IncreaseComboIndex();

	if (CachedPressTask)
	{
		CachedPressTask->EndTask();
		CachedPressTask = nullptr;
	}
}

void URsGameplayAbility_Combo::HandleInnerAbilityEnded(const FAbilityEndedData& AbilityEndData)
{
	if (InnerSpecHandles.Contains(AbilityEndData.AbilitySpecHandle))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}