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
	FRsAttributeMetaData();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttribute Attribute;

	// TODO: Change to FScalableFloat
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString DevComment;
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

public:
	template <typename T>
	static T* GetAttributeSet(UAbilitySystemComponent* ASC);
};

template <typename T>
T* URsAttributeSetBase::GetAttributeSet(UAbilitySystemComponent* ASC)
{
	static_assert(TIsDerivedFrom<T, URsAttributeSetBase>::IsDerived, "T must derive from URsAttributeSetBase");
	if (ASC)
	{
		for (UAttributeSet* AttributeSet : ASC->GetSpawnedAttributes())
		{
			if (T* Typed = Cast<T>(AttributeSet))
			{
				return Typed;
			}
		}
	}
	return nullptr;
}
