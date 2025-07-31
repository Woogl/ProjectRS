// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "BTTask_LockOn.generated.h"

/**
 * 
 */
UCLASS()
class RS_API UBTTask_LockOn : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_LockOn();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingShape Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingCollision Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingFilter Filter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingSorter Sorter;
};
