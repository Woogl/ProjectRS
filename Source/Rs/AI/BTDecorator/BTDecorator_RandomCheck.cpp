// Copyright 2024 Team BH.


#include "BTDecorator_RandomCheck.h"

bool UBTDecorator_RandomCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return FMath::RandRange(0.0f, 100.0f) < Probability;
}
