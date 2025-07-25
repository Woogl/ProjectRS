// Copyright 2024 Team BH.


#include "BTTask_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Rs/RsLogChannels.h"
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
			if (!ActivatedAbility.IsValid())
			{
				UE_LOG(RsLog, Error, TEXT("Character [%s] doesn't have Ability with tag [%s]"), *OwnerComp.GetAIOwner()->GetCharacter()->GetName(), *AbilityTags.ToString());
				return EBTNodeResult::Failed;
			}
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
	if (Ability)
	{
		// unbind delegate
		Ability->OnGameplayAbilityEnded.RemoveAll(this);
		if (Ability->IsActive())
		{
			if (AAIController* AIOwner = OwnerComp.GetAIOwner())
			{
				if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AIOwner))
				{
					ASC->CancelAbility(Ability);
				}
			}
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
