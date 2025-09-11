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
	//bool bDesiredCursorVisibility = false;

protected:
	virtual void ApplyUIInputConfig(const FUIInputConfig& NewConfig, bool bForceRefresh) override;
};
