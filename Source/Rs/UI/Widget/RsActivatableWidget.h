// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "RsActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class ERsWidgetInputMode : uint8
{
	Default		UMETA(ToolTip = "Input not set."),
	Game		UMETA(ToolTip = "Input from game only. Hides mouse cursor."),
	GameAndMenu	UMETA(ToolTip = "Input from both game and UI. Hides mouse cursor."),
	Menu		UMETA(ToolTip = "Input from UI only. Shows mouse cursor.")
};

UENUM(BlueprintType)
enum class ERsWidgetTimeControl : uint8
{
	Default,
	GamePause,
	TimeDilation,
};

/**
 * 
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
	UPROPERTY(EditDefaultsOnly, Category = "RS")
	ERsWidgetInputMode InputConfig = ERsWidgetInputMode::Default;

	UPROPERTY(EditDefaultsOnly, Category = "RS")
	ERsWidgetTimeControl TimeControl = ERsWidgetTimeControl::Default;

	UPROPERTY(EditDefaultsOnly, Category = "RS", meta = (EditCondition = "TimeControl == ERsWidgetTimeControl::TimeDilation", EditConditionHides))
	float TimeDilation = 0.01f;
	
	UPROPERTY(EditDefaultsOnly, Category = "RS", meta = (EditCondition = "TimeControl == ERsWidgetTimeControl::TimeDilation", EditConditionHides))
	float BlendTime = 0.f;
};
