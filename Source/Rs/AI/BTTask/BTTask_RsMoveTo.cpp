// Copyright 2025 Team BH.


#include "BTTask_RsMoveTo.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_RsMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (MoveSpeedMode == EMoveSpeedMode::Sprint)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner());
		if (ASC)
		{
			ASC->TryActivateAbilitiesByTag(SprintTag);
		}
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTTask_RsMoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if (MoveSpeedMode == EMoveSpeedMode::Sprint)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner());
		if (ASC)
		{
			ASC->CancelAbilities(&SprintTag);
		}
	}
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
