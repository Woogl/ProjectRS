// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/GameSettingScreen.h"

#include "RsSettingScreen.generated.h"

class URsTabListWidgetBase;
/**
 * 
 */
UCLASS(Abstract, meta = (Category = "Settings", DisableNativeTick))
class URsSettingScreen : public UGameSettingScreen
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual UGameSettingRegistry* CreateRegistry() override;

	void HandleBackAction();
	void HandleApplyAction();
	void HandleCancelChangesAction();

	virtual void OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = Input, meta = (BindWidget, OptionalWidget = true, AllowPrivateAccess = true))
	TObjectPtr<URsTabListWidgetBase> TopSettingsTabs;
	
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle BackInputActionData;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ApplyInputActionData;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle CancelChangesInputActionData;

	FUIActionBindingHandle BackHandle;
	FUIActionBindingHandle ApplyHandle;
	FUIActionBindingHandle CancelChangesHandle;
};
