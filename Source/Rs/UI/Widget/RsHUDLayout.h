// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "RsHUDLayout.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsHUDLayout : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddSystemMessage(const FText& Message, float Duration);
};
