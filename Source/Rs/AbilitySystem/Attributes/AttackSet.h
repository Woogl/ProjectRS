// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetBase.h"
#include "AttackSet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API UAttackSet : public URsAttributeSetBase
{
	GENERATED_BODY()

public:
	UAttackSet();

	// Value related to Damage dealt to enemies.
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos))
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UAttackSet, Attack)

	// Value related to Stagger gain to enemies
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos))
	FGameplayAttributeData Impact;
	ATTRIBUTE_ACCESSORS(UAttackSet, Impact)
};
