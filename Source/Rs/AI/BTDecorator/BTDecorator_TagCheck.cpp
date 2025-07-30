// Copyright 2024 Team BH.


#include "BTDecorator_TagCheck.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTDecorator_TagCheck::UBTDecorator_TagCheck()
{
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	Actor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_TagCheck, Actor), AActor::StaticClass());
	Actor.SelectedKeyName = FBlackboard::KeySelf;
	MatchType = EGameplayContainerMatchType::All;
}

bool UBTDecorator_TagCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (ASC.IsValid())
	{
		switch (MatchType)
		{
		case EGameplayContainerMatchType::All:
			return ASC->HasAllMatchingGameplayTags(Tags);

		case EGameplayContainerMatchType::Any:
			return ASC->HasAnyMatchingGameplayTags(Tags);
		}
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
	if (const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(Actor.GetSelectedKeyID()));
		if (TargetActor)
		{
			ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
			if (ASC.IsValid())
			{
				for (const FGameplayTag& Tag : Tags)
				{
					ASC->RegisterGameplayTagEvent(Tag).AddUObject(this, &UBTDecorator_TagCheck::OnTagAddedOrRemoved, &OwnerComp);
				}
			}
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
