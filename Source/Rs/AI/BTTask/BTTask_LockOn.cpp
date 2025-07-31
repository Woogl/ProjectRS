// Copyright 2025 Team BH.


#include "BTTask_LockOn.h"
#include "AIController.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"

UBTTask_LockOn::UBTTask_LockOn()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_LockOn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (URsLockOnComponent* LockOnComponent = OwnerComp.GetAIOwner()->FindComponentByClass<URsLockOnComponent>())
	{
		LockOnComponent->SetTargetingParams(Shape, Collision, Filter, Sorter);
		if (LockOnComponent->TryTargetingLockOn())
		{
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
