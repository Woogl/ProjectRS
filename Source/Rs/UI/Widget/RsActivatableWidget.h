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

UENUM(BlueprintType)
enum class ERsWidgetPauseMode : uint8
{
	Default,
	GamePause,
	TimeDilation,
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
	UPROPERTY(EditDefaultsOnly, Category = "RS")
	ERsWidgetInputMode InputConfig = ERsWidgetInputMode::Default;

	UPROPERTY(EditDefaultsOnly, Category = "RS")
	ERsWidgetPauseMode PauseControl = ERsWidgetPauseMode::Default;

	UPROPERTY(EditDefaultsOnly, Category = "RS", meta = (EditCondition = "PauseControl == ERsWidgetPauseMode::TimeDilation", EditConditionHides))
	float TimeDilation = 0.01f;
	
	UPROPERTY(EditDefaultsOnly, Category = "RS", meta = (EditCondition = "PauseControl == ERsWidgetPauseMode::TimeDilation", EditConditionHides))
	float BlendTime = 0.f;
};
