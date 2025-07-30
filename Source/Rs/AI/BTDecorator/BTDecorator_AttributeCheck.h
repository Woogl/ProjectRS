// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AttributeCheck.generated.h"

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

UENUM(BlueprintType)
enum class EThresholdValueType : uint8
{
	Numeric			UMETA(DisplayName = "Numeric Value(Float)"),
	Attribute		UMETA(DisplayName = "Other Attribute"),
	AttributeRatio	UMETA(DisplayName = "Ratio with Other Attribute (Check Attribute / Threshold Attribute)"),
};

UCLASS()
class RS_API UBTDecorator_AttributeCheck : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_AttributeCheck();
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeCheck|Target Value", meta = (DisplayName = "Actor to Check"))
	FBlackboardKeySelector Actor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeCheck|Target Value", meta = (DisplayName = "Attribute to Check"))
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeCheck|Operator")
	EAttributeInequalityOperator Operator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeCheck|Compare Value")
	EThresholdValueType ThresholdValueType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeCheck|Compare Value", meta = (EditConditionHides, EditCondition = "ThresholdValueType != EThresholdValueType::Numeric"))
	FGameplayAttribute ThresholdAttribute;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeCheck|Compare Value", meta = (EditConditionHides, EditCondition = "ThresholdValueType != EThresholdValueType::Attribute"))
	float Threshold;

protected:
	bool Compare(float TargetValue, float ThresholdValue) const;
};
