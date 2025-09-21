// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RsAttributeSetBase.generated.h"

// Use macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT(BlueprintType)
struct RS_API FRsAttributeMetaData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// float Level = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ATK = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float IMP = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DEF = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HPmax = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HPcur = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SPmax = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SPcur = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CRITrate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CRITdmg = 0.f;
	
	float GetValue(const FGameplayAttribute& Attribute) const;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChange, float, OldValue, float, NewValue, AActor*, Instigator);

/**
 * 
 */
UCLASS()
class RS_API URsAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

protected:
	void AdjustAttributeForMaxChange(const FGameplayAttribute& AffectedAttribute, float OldMaxValue, float NewMaxValue) const;
};
