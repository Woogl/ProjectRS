// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Rs/UI/Widget/RsActivatableWidget.h"
#include "RsHUDLayout.generated.h"

/**
 * Game layer widget (WBP_GameHUD)
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class RS_API URsHUDLayout : public URsActivatableWidget
{
	GENERATED_BODY()

public:
	// TODO: Refactoring
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddSystemMessage(const FText& Message, float Duration);
};
