// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RsLockOnInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URsLockOnInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RS_API IRsLockOnInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "RS Lock On")
	bool IsLockableTarget() const;
};
