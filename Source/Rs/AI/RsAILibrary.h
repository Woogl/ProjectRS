// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsAILibrary.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAILibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "RS AI Library", Meta=(DefaultToSelf = "Actor"))
	static uint8 GetTeamID(const AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "RS AI Library", Meta=(DefaultToSelf = "Actor"))
	static UObject* GetBlackboardValueAsObject(AActor* Actor, FName KeyName);
};
