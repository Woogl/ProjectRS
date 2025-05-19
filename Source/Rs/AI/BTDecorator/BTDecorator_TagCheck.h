// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_TagCheck.generated.h"

/**
 * 
 */
class UAbilitySystemComponent;

UCLASS()
class RS_API UBTDecorator_TagCheck : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_TagCheck();
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	void OnTagAddedOrRemoved(const FGameplayTag ChangedTag, int32 Stack, UBehaviorTreeComponent* OwnerComp);
	
protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Tags To Check (if exists, return true)"))
	FGameplayTagContainer Tags;

protected:
	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> ASC;
};
