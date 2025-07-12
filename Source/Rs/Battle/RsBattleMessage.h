// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsBattleMessage.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FRsLinkSkillMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TriggeredCharacter;
}; 

USTRUCT(BlueprintType)
struct FRsGameplayMessage
{
	GENERATED_BODY()
};