// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "RsAbilitySet.generated.h"

class UGameplayAbility;
class URsAttributeSetBase;
class UGameplayEffect;

// Contains data used to initialize an Ability System Component.
UCLASS()
class URsAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	// Attributes to set base values.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute", meta = (RowType = "RsAttributeTableRow"))
	FDataTableRowHandle GrantedAttributeTableRow;

	// Gameplay Abilities to give.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	// Gameplay Effects to apply.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect")
	TArray<TSubclassOf<UGameplayEffect>> GrantedEffects;

	// Gameplay Tags to add.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Tag")
	FGameplayTagContainer GrantedTags;
};