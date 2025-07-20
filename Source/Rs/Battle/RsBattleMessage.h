// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsBattleMessage.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FRsStateChangeMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Source;
}; 

USTRUCT(BlueprintType)
struct FRsGameplayMessage
{
	GENERATED_BODY()
};