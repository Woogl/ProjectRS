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
	// Attributes / float used to set base values.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute")
	TMap<FGameplayAttribute, FScalableFloat> GrantedAttributeValues;

	// Gameplay Abilities to give.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ability")
	TSet<TSubclassOf<URsGameplayAbility>> GrantedAbilities;

	// Gameplay Effects to apply.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect")
	TSet<TSubclassOf<UGameplayEffect>> GrantedEffects;

	// GameplayTags to add.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay Tag")
	FGameplayTagContainer GrantedTags;
};