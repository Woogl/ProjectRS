// Copyright Epic Games, Inc. All Rights Reserved.

#include "RsGameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "RsSettingsLocal.h"
#include "RsSettingsShared.h"
#include "Rs/Player/RsLocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RsGameSettingRegistry)

DEFINE_LOG_CATEGORY(LogRsGameSettingRegistry);

#define LOCTEXT_NAMESPACE "Rs"

//--------------------------------------
// URsGameSettingRegistry
//--------------------------------------

URsGameSettingRegistry::URsGameSettingRegistry()
{
}

URsGameSettingRegistry* URsGameSettingRegistry::Get(URsLocalPlayer* InLocalPlayer)
{
	URsGameSettingRegistry* Registry = FindObject<URsGameSettingRegistry>(InLocalPlayer, TEXT("RsGameSettingRegistry"), EFindObjectFlags::ExactClass);
	if (Registry == nullptr)
	{
		Registry = NewObject<URsGameSettingRegistry>(InLocalPlayer, TEXT("RsGameSettingRegistry"));
		Registry->Initialize(InLocalPlayer);
	}

	return Registry;
}

bool URsGameSettingRegistry::IsFinishedInitializing() const
{
	if (Super::IsFinishedInitializing())
	{
		if (URsLocalPlayer* LocalPlayer = Cast<URsLocalPlayer>(OwningLocalPlayer))
		{
			if (LocalPlayer->GetSharedSettings() == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

void URsGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	URsLocalPlayer* RsLocalPlayer = Cast<URsLocalPlayer>(InLocalPlayer);

	VideoSettings = InitializeVideoSettings(RsLocalPlayer);
	InitializeVideoSettings_FrameRates(VideoSettings, RsLocalPlayer);
	RegisterSetting(VideoSettings);

	AudioSettings = InitializeAudioSettings(RsLocalPlayer);
	RegisterSetting(AudioSettings);

	GameplaySettings = InitializeGameplaySettings(RsLocalPlayer);
	RegisterSetting(GameplaySettings);

	MouseAndKeyboardSettings = InitializeMouseAndKeyboardSettings(RsLocalPlayer);
	RegisterSetting(MouseAndKeyboardSettings);

	GamepadSettings = InitializeGamepadSettings(RsLocalPlayer);
	RegisterSetting(GamepadSettings);
}

void URsGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();
	
	if (URsLocalPlayer* LocalPlayer = Cast<URsLocalPlayer>(OwningLocalPlayer))
	{
		// Game user settings need to be applied to handle things like resolution, this saves indirectly
		LocalPlayer->GetLocalSettings()->ApplySettings(false);
		
		LocalPlayer->GetSharedSettings()->ApplySettings();
		LocalPlayer->GetSharedSettings()->SaveSettings();
	}
}

#undef LOCTEXT_NAMESPACE

