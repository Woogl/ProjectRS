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
	None,
	Forward,
	Backward,
	Leftward,
	Rightward,
};

UCLASS()
class RS_API UBTTask_RandomMove : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RandomMove();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FVector GetRandomReachablePointInAngle(const FVector& Origin, const FVector& Forward, float MinAngle, float MaxAngle);
	UFUNCTION()
	void OnMoveFinished(FAIRequestID RequestID, EPathFollowingResult::Type Result);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoveDirection Direction = EMoveDirection::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance = 300.f;

private:
	TWeakObjectPtr<UBehaviorTreeComponent> MyOwnerComp;

};
