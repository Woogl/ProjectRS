// Copyright 2024 Team BH.


#include "BTDecorator_StatusCheck.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"

bool UBTDecorator_StatusCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner());
	if (ASC)
	{
		bool FoundResult;
		float FoundValue = ASC->GetGameplayAttributeValue(Attribute, FoundResult);

		if (FoundResult)
		{
			switch (Operator)
			{
			case EAttributeInequalityOperator::Equal:
				return FMath::IsNearlyEqual(FoundValue, Value, 0.4);
			case EAttributeInequalityOperator::Less:
				return FoundValue < Value;
			case EAttributeInequalityOperator::LessEqual:
				return FoundValue <= Value;
			case EAttributeInequalityOperator::Greater:
				return FoundValue > Value;
			case EAttributeInequalityOperator::GreaterEqual:
				return FoundValue >= Value;
			}
		}
	}
	return false;
}
