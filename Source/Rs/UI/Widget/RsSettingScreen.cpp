// Copyright 2025 Team BH.


#include "RsSettingScreen.h"

#include "Input/CommonUIInputTypes.h"
#include "Rs/Player/RsLocalPlayer.h"
#include "Rs/Settings/RsGameSettingRegistry.h"

void URsSettingScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleBackAction)));
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleApplyAction)));
	CancelChangesHandle = RegisterUIActionBinding(FBindUIActionArgs(CancelChangesInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleCancelChangesAction)));
}

UGameSettingRegistry* URsSettingScreen::CreateRegistry()
{
	URsGameSettingRegistry* NewRegistry = NewObject<URsGameSettingRegistry>();
	
	if (URsLocalPlayer* LocalPlayer = CastChecked<URsLocalPlayer>(GetOwningLocalPlayer()))
	{
		NewRegistry->Initialize(LocalPlayer);
	}
	
	return NewRegistry;
}

void URsSettingScreen::HandleBackAction()
{
	if (AttemptToPopNavigation())
	{
		return;
	}

	ApplyChanges();

	DeactivateWidget();
}

void URsSettingScreen::HandleApplyAction()
{
	ApplyChanges();
}

void URsSettingScreen::HandleCancelChangesAction()
{
	CancelChanges();
}

void URsSettingScreen::OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty)
{
	if (bSettingsDirty)
	{
		if (!GetActionBindings().Contains(ApplyHandle))
		{
			AddActionBinding(ApplyHandle);
		}
		if (!GetActionBindings().Contains(CancelChangesHandle))
		{
			AddActionBinding(CancelChangesHandle);
		}
	}
	else
	{
		RemoveActionBinding(ApplyHandle);
		RemoveActionBinding(CancelChangesHandle);
	}
}
