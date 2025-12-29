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

#define RS_CAPTURE_ATTRIBUTE(Attr, SourceOrTarget, Snapshot) \
{ \
FGameplayEffectAttributeCaptureDefinition Def; \
Def.AttributeToCapture = (Attr); \
Def.AttributeSource = (SourceOrTarget); \
Def.bSnapshot = Snapshot; \
this->RelevantAttributesToCapture.Add(Def); \
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRsAttributeChangedEvent, float, OldValue, float, NewValue);

/**
 * 
 */
UCLASS()
class RS_API URsAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	static FGameplayAttribute TagToAttribute(const FGameplayTag& Tag);
	static FGameplayTag AttributeToTag(const FGameplayAttribute& Attribute);
	static const TMap<FGameplayTag, FGameplayAttribute>& GetStatMap();
	static const TMap<FGameplayTag, FGameplayAttribute>& GetCoefficientMap();
	
protected:
	void AdjustAttributeForMaxChange(const FGameplayAttribute& AffectedAttribute, float OldMaxValue, float NewMaxValue) const;
	
	// Registers a Tag-to-Stat mapping on the CDO.
	void RegisterTagToStat(const FGameplayTag& Tag, FGameplayAttribute Attribute);
	
	// Registers a Tag-to-Coefficient mapping on the CDO.
	void RegisterTagToCoefficient(const FGameplayTag& Tag, FGameplayAttribute Attribute);
	
private:
	inline static TMap<FGameplayTag, FGameplayAttribute> TagStatMap;
	inline static TMap<FGameplayTag, FGameplayAttribute> TagCoefficientMap;
	inline static TMap<FGameplayAttribute, FGameplayTag> StatTagMap;
};
