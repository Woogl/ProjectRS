// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "AbilitySystemData.generated.h"

class URsGameplayAbility;
class URsAttributeSetBase;
class UGameplayEffect;

// Contains data used to initialize an Ability System Component.
USTRUCT(BlueprintType)
struct FAbilitySystemInitializationData
{
	GENERATED_BODY()

	// An array of Attribute Sets to create.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSet<TSubclassOf<URsAttributeSetBase>> AttributeSets;

	// A map of Attributes / float used to set base values.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<FGameplayAttribute, FScalableFloat> AttributeBaseValues;

	// An Array of Gameplay Abilities to give.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSet<TSubclassOf<URsGameplayAbility>> GameplayAbilities;

	// An array of Gameplay Effects to apply.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSet<TSubclassOf<UGameplayEffect>> GameplayEffects;

	// A container of GameplayTags to apply.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTagContainer GameplayTags;
};