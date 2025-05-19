// Copyright 2024 Team BH.


#include "BTTask_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"

UBTTask_ActivateAbility::UBTTask_ActivateAbility()
{
	bCreateNodeInstance = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner()))
	{
		if (ASC->TryActivateAbilitiesByTag(AbilityTags))
		{
			ActivatedAbility = URsAbilitySystemLibrary::FindAbilityWithTag(ASC, AbilityTags, true);
			if (ActivatedAbility->IsActive())
			{
				ActivatedAbility->OnGameplayAbilityEnded.AddUObject(this, &ThisClass::HandleAbilityEnded, &OwnerComp);
				Result = EBTNodeResult::InProgress;
			}
		}
	}
	return Result;
}

void UBTTask_ActivateAbility::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	UGameplayAbility* Ability = ActivatedAbility.Get();
	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (Ability && AIOwner)
	{
		// unbind delegate
		Ability->OnGameplayAbilityEnded.RemoveAll(this);
		if (Ability->IsActive())
		{
			UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AIOwner)->CancelAbility(Ability);
		}
	}
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_ActivateAbility::HandleAbilityEnded(UGameplayAbility* Ability, UBehaviorTreeComponent* OwnerComp)
{
	if (IsValid(OwnerComp))
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
}
