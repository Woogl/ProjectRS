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
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute", meta = (RowType = "RsAttributeMetaData"))
	FDataTableRowHandle GrantedAttributeTableRow;
	
	// Attributes / float used to set base values.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute")
	TMap<FGameplayAttribute, FScalableFloat> GrantedAttributes;

	// Gameplay Abilities to give.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ability")
	TSet<TSubclassOf<URsGameplayAbility>> GrantedAbilities;

	// Gameplay Effects to apply.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect")
	TSet<TSubclassOf<UGameplayEffect>> GrantedEffects;

	// Gameplay Tags to add.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Tag")
	FGameplayTagContainer GrantedTags;
};