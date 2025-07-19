// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "RsAbilityEventInfo.generated.h"

class URsEffectDefinition;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRsAbilityEventInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories = "Animnotify"))
	FGameplayTag EventTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<URsEffectDefinition*> EffectDefinitions;

	bool operator==(const FRsAbilityEventInfo& Other) const
	{
		return this->EventTag == Other.EventTag;
	}
	
	bool operator==(const FGameplayTag& Other) const
	{
		return this->EventTag == Other;
	}
};