// Copyright 2024 Team BH.


#include "BTTask_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"

UBTTask_ActivateAbility::UBTTask_ActivateAbility()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner()))
	{
		if (ASC->TryActivateAbilitiesByTag(AbilityTags))
		{
			ActivatedAbility = URsAbilitySystemLibrary::FindAbilityWithTag(ASC, AbilityTags, true);
			if (ActivatedAbility.IsValid())
			{
				if (ActivatedAbility->IsActive())
				{
					MyOwnerComp = &OwnerComp;
					ActivatedAbility->OnGameplayAbilityEnded.AddUObject(this, &ThisClass::HandleAbilityEnded);
					Result = EBTNodeResult::InProgress;
				}
			}
		}
	}
	return Result;
}

void UBTTask_ActivateAbility::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if (ActivatedAbility.IsValid())
	{
		ActivatedAbility->OnGameplayAbilityEnded.RemoveAll(this);
	}
}

void UBTTask_ActivateAbility::HandleAbilityEnded(UGameplayAbility* Ability)
{
	FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);
}
