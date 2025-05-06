// Copyright 2024 Team BH.


#include "BTTask_SetBBValue.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_NativeEnum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_SetBBValue::UBTTask_SetBBValue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ObjectValue.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SetBBValue, ObjectValue), UObject::StaticClass());
}

EBTNodeResult::Type UBTTask_SetBBValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardAsset) return EBTNodeResult::Failed;

	BlackboardKey.ResolveSelectedKey(*BlackboardAsset);

	const FBlackboardEntry* EntryInfo = BlackboardAsset ? BlackboardAsset->GetKey(BlackboardKey.GetSelectedKeyID()) : NULL;

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass())
	{
		BlackboardComponent->SetValueAsBool(BlackboardKey.SelectedKeyName, BoolValue);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Int::StaticClass())
	{
		int32 ResultInt =
			IsMathOperationRequired() ? BlackboardComponent->GetValueAsInt(BlackboardKey.SelectedKeyName) : IntValue;
		BlackboardComponent->SetValueAsInt(BlackboardKey.SelectedKeyName, ResultInt);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
	{
		float ResultFloat =
			IsMathOperationRequired() ? DoMathOperation(BlackboardComponent->GetValueAsFloat(BlackboardKey.SelectedKeyName)) : FloatValue;
		BlackboardComponent->SetValueAsFloat(BlackboardKey.SelectedKeyName, ResultFloat);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_String::StaticClass())
	{
		BlackboardComponent->SetValueAsString(BlackboardKey.SelectedKeyName, StringValue);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Name::StaticClass())
	{
		BlackboardComponent->SetValueAsName(BlackboardKey.SelectedKeyName, NameValue);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		BlackboardComponent->SetValueAsVector(BlackboardKey.SelectedKeyName, VectorValue);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		BlackboardComponent->SetValueAsRotator(BlackboardKey.SelectedKeyName, RotatorValue);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Enum::StaticClass())
	{
		CurrentEnum = ((UBlackboardKeyType_Enum*)(EntryInfo->KeyType))->EnumType;
	    if (CurrentEnum)
	    {
	        BlackboardComponent->SetValueAsEnum(BlackboardKey.SelectedKeyName, CurrentEnum->GetValueByNameString(EnumStringValue));
	    }
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_NativeEnum::StaticClass())
	{
		CurrentEnum = ((UBlackboardKeyType_NativeEnum*)(EntryInfo->KeyType))->EnumType;
	    if (CurrentEnum)
	    {
	        BlackboardComponent->SetValueAsEnum(BlackboardKey.SelectedKeyName, CurrentEnum->GetValueByNameString(EnumStringValue));
	    }
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		if (ObjectValue.NeedsResolving())
		{
			ObjectValue.ResolveSelectedKey(*BlackboardAsset);
		}
		BlackboardComponent->SetValueAsObject(BlackboardKey.SelectedKeyName, BlackboardComponent->GetValueAsObject(ObjectValue.SelectedKeyName));
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Class::StaticClass())
	{
		BlackboardComponent->SetValueAsClass(BlackboardKey.SelectedKeyName, ClassValue);
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

FString UBTTask_SetBBValue::GetStaticDescription() const
{
	FString Description = FString();

	switch (CurrentBBKeyValueType)
	{
		case EBBValueType::Bool:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), BoolValue ? TEXT("true") : TEXT("false"));
			break;
		case EBBValueType::Int:
			Description = FString::Printf(TEXT("Set \"%s\" to %d"), *BlackboardKey.SelectedKeyName.ToString(), IntValue);
			break;
		case EBBValueType::Float:
			Description = FString::Printf(TEXT("Set \"%s\" to %f"), *BlackboardKey.SelectedKeyName.ToString(), FloatValue);
			break;
		case EBBValueType::String:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *StringValue);
			break;
		case EBBValueType::Name:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *NameValue.ToString());
			break;
		case EBBValueType::Vector:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *VectorValue.ToString());
			break;
		case EBBValueType::Rotator:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *RotatorValue.ToString());
			break;
		case EBBValueType::Enum:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *EnumStringValue);
			break;
		case EBBValueType::NativeEnum:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *EnumStringValue);
			break;
		case EBBValueType::Object:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *ObjectValue.SelectedKeyName.ToString());
			break;
		case EBBValueType::Class:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), ClassValue ? *ClassValue->GetName() : TEXT(""));
			break;
		default:
			break;
	}

	// return FString::Printf(TEXT("%s: \n%s"), *Super::GetStaticDescription(), *Description);
	return FString::Printf(TEXT("%s"), *Description);
}

#if WITH_EDITOR
void UBTTask_SetBBValue::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	if (!BlackboardAsset) return;

	if (!BlackboardKey.IsSet()) return;
	BlackboardKey.ResolveSelectedKey(*BlackboardAsset);
	const FBlackboardEntry* EntryInfo = BlackboardAsset ? BlackboardAsset->GetKey(BlackboardKey.GetSelectedKeyID()) : NULL;

	CurrentBBKeyValueType = BlackboardKeyToBBValueType(BlackboardKey);
}
#endif

TArray<FString> UBTTask_SetBBValue::GetEnumOptions()
{
	TArray<FString> Result = {};

	const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	if (!BlackboardAsset) return Result;

	BlackboardKey.ResolveSelectedKey(*BlackboardAsset);
	const FBlackboardEntry* EntryInfo = BlackboardAsset ? BlackboardAsset->GetKey(BlackboardKey.GetSelectedKeyID()) : NULL;

	const UEnum* Enum = (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Enum::StaticClass())
		                    ? ((UBlackboardKeyType_Enum*)(EntryInfo->KeyType))->EnumType
		                    : ((UBlackboardKeyType_NativeEnum*)(EntryInfo->KeyType))->EnumType;

	if (!Enum || !Enum->IsValidLowLevel())
	{
		return Result;
	}

	for (int32 i = 0; i < Enum->NumEnums(); i++)
	{
		Result.Add(Enum->GetNameByIndex(i).ToString());
	}

	return Result;
}

EBBValueType UBTTask_SetBBValue::BlackboardKeyToBBValueType(FBlackboardKeySelector Key)
{
	EBBValueType Result = EBBValueType::None;

	if (Key.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass())
	{
		Result = EBBValueType::Bool;
	}
	if (Key.SelectedKeyType == UBlackboardKeyType_Int::StaticClass())
	{
		Result = EBBValueType::Int;
	}
	if (Key.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
	{
		Result = EBBValueType::Float;
	}
	if (Key.SelectedKeyType == UBlackboardKeyType_String::StaticClass())
	{
		Result = EBBValueType::String;
	}
	if (Key.SelectedKeyType == UBlackboardKeyType_Name::StaticClass())
	{
		Result = EBBValueType::Name;
	}
	if (Key.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		Result = EBBValueType::Vector;
	}
	if (Key.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		Result = EBBValueType::Rotator;
	}
	if (Key.SelectedKeyType == UBlackboardKeyType_Enum::StaticClass())
	{
		Result = EBBValueType::Enum;
	}
	if (Key.SelectedKeyType == UBlackboardKeyType_NativeEnum::StaticClass())
	{
		Result = EBBValueType::NativeEnum;
	}
	if (Key.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		Result = EBBValueType::Object;
	}
	if (Key.SelectedKeyType == UBlackboardKeyType_Class::StaticClass())
	{
		Result = EBBValueType::Class;
	}
	return Result;
}

int32 UBTTask_SetBBValue::DoMathOperation(int32 BaseValue)
{
	if (MathOperation == ESetBBValue_MathOperations::Add)
	{
		return BaseValue + IntValue;
	}
	if (MathOperation == ESetBBValue_MathOperations::Multiply)
	{
		return BaseValue * IntValue;
	}
	if (MathOperation == ESetBBValue_MathOperations::Divide)
	{
		return BaseValue / IntValue;
	}
	return BaseValue;
}

float UBTTask_SetBBValue::DoMathOperation(float BaseValue)
{
	if (MathOperation == ESetBBValue_MathOperations::Add)
	{
		return BaseValue + FloatValue;
	}
	if (MathOperation == ESetBBValue_MathOperations::Multiply)
	{
		return BaseValue * FloatValue;
	}
	if (MathOperation == ESetBBValue_MathOperations::Divide)
	{
		return BaseValue / FloatValue;
	}
	return BaseValue;
}

