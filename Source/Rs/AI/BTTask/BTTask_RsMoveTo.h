// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_RsMoveTo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMoveSpeedMode : uint8
{
	Walk,
	Sprint,
};

UCLASS()
class RS_API UBTTask_RsMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta = (AllowPrivateAccess = true))
	EMoveSpeedMode MoveSpeedMode = EMoveSpeedMode::Walk;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = true))
	FGameplayTagContainer SprintTag = FGameplayTag::RequestGameplayTag("Ability.Sprint").GetSingleTagContainer();
};
