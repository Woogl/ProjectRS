// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RsTargetingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class URsTargetingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RS_API IRsTargetingInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "RS|Targeting")
	bool IsTargetable() const;
};