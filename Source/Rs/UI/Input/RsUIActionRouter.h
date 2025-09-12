// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Input/CommonUIActionRouterBase.h"
#include "RsUIActionRouter.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsUIActionRouter : public UCommonUIActionRouterBase
{
	GENERATED_BODY()

public:
	void ApplyLeafmostNodeConfig();
};
