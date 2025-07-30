// Copyright 2024 Team BH.


#include "BTDecorator_AttributeCheck.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"

UBTDecorator_AttributeCheck::UBTDecorator_AttributeCheck()
{
}

bool UBTDecorator_AttributeCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner());
	if (ASC)
	{
		bool FoundResult;
		float FoundValue = ASC->GetGameplayAttributeValue(Attribute, FoundResult);

		if (FoundResult)
		{
			if (ThresholdValueType == EThresholdValueType::Numeric)
			{
				return Compare(FoundValue, Threshold);
			}

			float CompareValue = ASC->GetGameplayAttributeValue(ThresholdAttribute, FoundResult);
			if (FoundResult)
			{
				switch (ThresholdValueType)
				{
				case EThresholdValueType::Attribute:
					return Compare(FoundValue, CompareValue);
				case EThresholdValueType::AttributeRatio:
					return Compare(FoundValue / CompareValue, Threshold);
				default:
					break;
				}
			}
		}
	}
	return false;
}

bool UBTDecorator_AttributeCheck::Compare(float TargetValue, float ThresholdValue) const
{
	switch (Operator)
	{
	case EAttributeInequalityOperator::Equal:
		return FMath::IsNearlyEqual(TargetValue, ThresholdValue, 0.4);
	case EAttributeInequalityOperator::Less:
		return TargetValue < ThresholdValue;
	case EAttributeInequalityOperator::LessEqual:
		return TargetValue <= ThresholdValue;
	case EAttributeInequalityOperator::Greater:
		return TargetValue > ThresholdValue;
	case EAttributeInequalityOperator::GreaterEqual:
		return TargetValue >= ThresholdValue;
	}
	return false;
}
