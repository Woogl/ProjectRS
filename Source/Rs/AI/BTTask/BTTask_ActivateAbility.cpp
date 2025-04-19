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
			if (UGameplayAbility* ActivatedAbility = URsAbilitySystemLibrary::FindAbilityWithTag(ASC, AbilityTags, true))
			{
				if (ActivatedAbility->IsActive())
				{
					MyOwnerComp = &OwnerComp;
					ActivatedAbility->OnGameplayAbilityEnded.AddWeakLambda(this, [this](UGameplayAbility* Ability)
					{
						if (this && MyOwnerComp.IsValid())
						{
							FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);
						}
					});
					Result = EBTNodeResult::InProgress;
				}
			}
		}
	}
	return Result;
}
