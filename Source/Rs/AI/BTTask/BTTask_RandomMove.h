// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_RandomMove.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	Any,
	Forward,
	Backward,
	Left,
	Right,
};

UCLASS()
class RS_API UBTTask_RandomMove : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RandomMove();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FVector GetRandomReachablePointInAngle(const FVector& Origin, const FVector& Forward, float MinAngle, float MaxAngle);

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
	void OnMoveFinished(FAIRequestID RequestID, const FPathFollowingResult& Result, UBehaviorTreeComponent* OwnerComp);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoveDirection Direction = EMoveDirection::Any;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDistance = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDistance = 300.f;
};
