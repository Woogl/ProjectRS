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

DECLARE_MULTICAST_DELEGATE_OneParam(FRsAttributeEvent, const FOnAttributeChangeData&);

/**
 * 
 */
UCLASS()
class RS_API URsAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	static FGameplayAttribute TagToAttribute(const FGameplayTag& Tag);
	static const TMap<FGameplayTag, FGameplayAttribute>& GetStatMap();
	static const TMap<FGameplayTag, FGameplayAttribute>& GetCoefficientMap();
	
protected:
	void AdjustAttributeForMaxChange(const FGameplayAttribute& AffectedAttribute, float OldMaxValue, float NewMaxValue) const;
	
	// Registers a Tag-to-Stat mapping on the CDO.
	void RegisterTagToStat(const FGameplayTag& Tag, FGameplayAttribute Attribute);
	
	// Registers a Tag-to-Coefficient mapping on the CDO.
	void RegisterTagToCoefficient(const FGameplayTag& Tag, FGameplayAttribute Attribute);
	
private:
	inline static TMap<FGameplayTag, FGameplayAttribute> StatMap;
	inline static TMap<FGameplayTag, FGameplayAttribute> CoefficientMap;
};
