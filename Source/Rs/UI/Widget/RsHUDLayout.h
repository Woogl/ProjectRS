// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsActivatableWidget.h"
#include "RsHUDLayout.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class RS_API URsHUDLayout : public URsActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddSystemMessage(const FText& Message, float Duration);
};
