// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateAbility.generated.h"

/**
 * 
 */
UCLASS()
class RS_API UBTTask_ActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ActivateAbility();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta = (Categories = "Ability"))
	FGameplayTagContainer AbilityTags;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> MyOwnerComp;
};
