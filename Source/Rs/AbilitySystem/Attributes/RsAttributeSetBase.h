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
	float MaxHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Barrier = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxStagger = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentStagger = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StaggerDecay = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMana = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentMana = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxEnergy = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentEnergy = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Attack = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Impact = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Defense = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CriticalRate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CriticalDamage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Invincible = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SuperArmor = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveSpeed  = 1.f;
	
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
