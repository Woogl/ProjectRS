// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsAILibrary.generated.h"

struct FGenericTeamId;
/**
 * 
 */
UCLASS()
class RS_API URsAILibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rs AI Library")
	static uint8 GetTeamID(AActor* Actor);
};
