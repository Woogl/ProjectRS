// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "RsAbilitySet.generated.h"

class URsGameplayAbility;
class URsAttributeSetBase;
class UGameplayEffect;

// Contains data used to initialize an Ability System Component.
UCLASS()
class URsAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	// An array of Attribute Sets to create.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute")
	TSet<TSubclassOf<URsAttributeSetBase>> GrantedAttributes;

	// A map of Attributes / float used to set base values.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute")
	TMap<FGameplayAttribute, FScalableFloat> GrantedAttributeValues;

	// An Array of Gameplay Abilities to give.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ability")
	TSet<TSubclassOf<URsGameplayAbility>> GrantedAbilities;

	// An array of Gameplay Effects to apply.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect")
	TSet<TSubclassOf<UGameplayEffect>> GrantedEffects;

	// A container of GameplayTags to apply.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay Tag")
	FGameplayTagContainer GrantedTags;
};