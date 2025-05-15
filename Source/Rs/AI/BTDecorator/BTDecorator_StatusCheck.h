// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_StatusCheck.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAttributeInequalityOperator : uint8
{
	Equal,
	Less,
	LessEqual,
	Greater,
	GreaterEqual,
};

UCLASS()
class RS_API UBTDecorator_StatusCheck : public UBTDecorator
{
	GENERATED_BODY()

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttributeInequalityOperator Operator;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;

	
};
