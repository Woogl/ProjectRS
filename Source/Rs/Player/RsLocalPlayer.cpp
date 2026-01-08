// Copyright Epic Games, Inc. All Rights Reserved.

#include "RsLocalPlayer.h"

#include "AudioMixerBlueprintLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Rs/Settings/RsSettingsLocal.h"
#include "CommonUserSubsystem.h"
#include "Rs/Settings/RsSettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RsLocalPlayer)

class UObject;

URsLocalPlayer::URsLocalPlayer()
{
}

void URsLocalPlayer::PostInitProperties()
{
	Super::PostInitProperties();

	if (URsSettingsLocal* LocalSettings = GetLocalSettings())
	{
		LocalSettings->OnAudioOutputDeviceChanged.AddUObject(this, &URsLocalPlayer::OnAudioOutputDeviceChanged);
	}
}

void URsLocalPlayer::SwitchController(class APlayerController* PC)
{
	Super::SwitchController(PC);

	// OnPlayerControllerChanged(PlayerController);
}

bool URsLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
	const bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);

	// OnPlayerControllerChanged(PlayerController);

	return bResult;
}

void URsLocalPlayer::InitOnlineSession()
{
	// OnPlayerControllerChanged(PlayerController);
	
	Super::InitOnlineSession();
}

URsSettingsLocal* URsLocalPlayer::GetLocalSettings() const
{
	return URsSettingsLocal::Get();
}

URsSettingsShared* URsLocalPlayer::GetSharedSettings() const
{
	if (!SharedSettings)
	{
		// On PC it's okay to use the sync load because it only checks the disk
		// This could use a platform tag to check for proper save support instead
		bool bCanLoadBeforeLogin = PLATFORM_DESKTOP;
		
		if (bCanLoadBeforeLogin)
		{
			SharedSettings = URsSettingsShared::LoadOrCreateSettings(this);
		}
		else
		{
			// We need to wait for user login to get the real settings so return temp ones
			SharedSettings = URsSettingsShared::CreateTemporarySettings(this);
		}
	}

	return SharedSettings;
}

void URsLocalPlayer::LoadSharedSettingsFromDisk(bool bForceLoad)
{
	FUniqueNetIdRepl CurrentNetId = GetCachedUniqueNetId();
	if (!bForceLoad && SharedSettings && CurrentNetId == NetIdForSharedSettings)
	{
		// Already loaded once, don't reload
		return;
	}

	ensure(URsSettingsShared::AsyncLoadOrCreateSettings(this, URsSettingsShared::FOnSettingsLoadedEvent::CreateUObject(this, &URsLocalPlayer::OnSharedSettingsLoaded)));
}

void URsLocalPlayer::OnSharedSettingsLoaded(URsSettingsShared* LoadedOrCreatedSettings)
{
	// The settings are applied before it gets here
	if (ensure(LoadedOrCreatedSettings))
	{
		// This will replace the temporary or previously loaded object which will GC out normally
		SharedSettings = LoadedOrCreatedSettings;

		NetIdForSharedSettings = GetCachedUniqueNetId();
	}
}

void URsLocalPlayer::OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId)
{
	FOnCompletedDeviceSwap DevicesSwappedCallback;
	DevicesSwappedCallback.BindUFunction(this, FName("OnCompletedAudioDeviceSwap"));
	UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(GetWorld(), InAudioOutputDeviceId, DevicesSwappedCallback);
}

void URsLocalPlayer::OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult)
{
	if (SwapResult.Result == ESwapAudioOutputDeviceResultState::Failure)
	{
	}
}

