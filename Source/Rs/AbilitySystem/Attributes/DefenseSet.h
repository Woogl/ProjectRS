// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetBase.h"
#include "DefenseSet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API UDefenseSet : public URsAttributeSetBase
{
	GENERATED_BODY()

public:
	UDefenseSet();

	// Value related to damage reduction when attacked
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos))
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UDefenseSet, Defense)

	// Value related to stagger gain reduction when attacked
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos))
	FGameplayAttributeData Will;
	ATTRIBUTE_ACCESSORS(UDefenseSet, Will)
};
