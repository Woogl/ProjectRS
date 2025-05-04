// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateAbility.generated.h"

class UGameplayAbility;
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
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	void HandleAbilityEnded(UGameplayAbility* Ability);
	
	TWeakObjectPtr<UBehaviorTreeComponent> MyOwnerComp;
	TWeakObjectPtr<UGameplayAbility> ActivatedAbility;
};
