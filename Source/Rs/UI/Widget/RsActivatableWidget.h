// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "RsActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class ERsWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

/**
 * An activatable widget that automatically drives the desired input config when activated
 */
UCLASS(Abstract, Blueprintable)
class RS_API URsActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

protected:
	/** The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
	UPROPERTY(EditDefaultsOnly, Category = "RS|Input")
	ERsWidgetInputMode InputConfig = ERsWidgetInputMode::Default;
};
