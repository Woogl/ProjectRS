// Copyright 2024 Team BH.


#include "BTDecorator_TagCheck.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"

UBTDecorator_TagCheck::UBTDecorator_TagCheck()
{
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

bool UBTDecorator_TagCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (ASC.IsValid() && ASC->HasAllMatchingGameplayTags(Tags))
	{
		return true;
	}
	return false;
}

void UBTDecorator_TagCheck::OnTagAddedOrRemoved(const FGameplayTag, int32 Stack, UBehaviorTreeComponent* OwnerComp)
{
	if (IsValid(OwnerComp))
	{
		OwnerComp->RequestExecution(this);
	}
}

void UBTDecorator_TagCheck::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner()->GetPawn());
	if (ASC.IsValid())
	{
		for (const FGameplayTag& Tag : Tags)
		{
			ASC->RegisterGameplayTagEvent(Tag).AddUObject(this, &UBTDecorator_TagCheck::OnTagAddedOrRemoved, &OwnerComp);
		}
	}
}

void UBTDecorator_TagCheck::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASC.IsValid())
	{
		for (const FGameplayTag& Tag : Tags)
		{
			ASC->RegisterGameplayTagEvent(Tag).RemoveAll(this);
		}
	}
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}
