// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetBBValue.generated.h"

/**
 * 
 */
UENUM()
enum class EBBValueType
{
	None,
	Bool,
	Int,
	Float,
	String,
	Name,
	Vector,
	Rotator,
	Enum,
	NativeEnum,
	Object,
	Class,
};

UENUM(BlueprintType)
enum class ESetBBValue_MathOperations : uint8
{
	Override,
	Add,
	Multiply,
	Divide,
};

UCLASS()
class RS_API UBTTask_SetBBValue : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SetBBValue(const FObjectInitializer& ObjectInitializer);

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	struct FBlackboardKeySelector BlackboardKey;

    // TODO:
    // UPROPERTY(Category="Blackboard", EditAnywhere, meta=(EditCondition="CurrentBBKeyValueType==EBBValueType::Int"))
    // bool bDoMath = false;
    // UPROPERTY(Category="Blackboard", EditAnywhere, meta=(EditCondition="bDoMath && CurrentBBKeyValueType==EBBValueType::Int", EditConditionHides))
    UPROPERTY(Category="Blackboard", EditAnywhere, meta=(EditCondition="CurrentBBKeyValueType==EBBValueType::Int||CurrentBBKeyValueType==EBBValueType::Float", EditConditionHides))
    ESetBBValue_MathOperations MathOperation = ESetBBValue_MathOperations::Override;

	/** Values */
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Bool", EditConditionHides))
	bool BoolValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Int", EditConditionHides))
	int32 IntValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Float", EditConditionHides))
	float FloatValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::String", EditConditionHides, MultiLine))
	FString StringValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Name", EditConditionHides, MultiLine))
	FName NameValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Vector", EditConditionHides))
	FVector VectorValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Rotator", EditConditionHides))
	FRotator RotatorValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Enum || CurrentBBKeyValueType==EBBValueType::NativeEnum", EditConditionHides, GetOptions="GetEnumOptions"))
	FString EnumStringValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Object", EditConditionHides))
	FBlackboardKeySelector ObjectValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Class", EditConditionHides))
	UClass* ClassValue;
	/** ~Values */

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;

#if WITH_EDITORONLY_DATA

	// UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Query"))
	// TEnumAsByte<EBasicKeyOperation::Type> BasicOperation;
	// UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Query"))
	// TEnumAsByte<EArithmeticKeyOperation::Type> ArithmeticOperation;
	// UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Query"))
	// TEnumAsByte<ETextKeyOperation::Type> TextOperation;

#endif

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UPROPERTY()
	UEnum* CurrentEnum;
	UPROPERTY()
	EBBValueType CurrentBBKeyValueType = EBBValueType::None;

    UFUNCTION()
    TArray<FString> GetEnumOptions();
    bool IsMathOperationRequired() const {
        return MathOperation == ESetBBValue_MathOperations::Add
            || MathOperation == ESetBBValue_MathOperations::Multiply
            || MathOperation == ESetBBValue_MathOperations::Divide;
    };

	UFUNCTION()
	static EBBValueType BlackboardKeyToBBValueType(FBlackboardKeySelector Key);


	int32 DoMathOperation(int32 BaseValue);
	float DoMathOperation(float BaseValue);
};
