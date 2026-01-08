// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "Engine/PlatformSettings.h"
#include "GameplayTagContainer.h"

#include "RsPerformanceSettings.generated.h"

enum class ERsDisplayablePerformanceStat : uint8;

class UObject;

// Way to display the stat
UENUM(BlueprintType)
enum class ERsStatDisplayMode : uint8
{
	// Don't show this stat
	Hidden,

	// Show this stat in text form
	TextOnly,

	// Show this stat in graph form
	GraphOnly,

	// Show this stat as both text and graph
	TextAndGraph
};

//////////////////////////////////////////////////////////////////////

// Different kinds of stats that can be displayed on-screen
UENUM(BlueprintType)
enum class ERsDisplayablePerformanceStat : uint8
{
	// stat fps (in Hz)
	ClientFPS,

	// server tick rate (in Hz)
	ServerFPS,
	
	// idle time spent waiting for vsync or frame rate limit (in seconds)
	IdleTime,

	// Stat unit overall (in seconds)
	FrameTime,

	// Stat unit (game thread, in seconds)
	FrameTime_GameThread,

	// Stat unit (render thread, in seconds)
	FrameTime_RenderThread,

	// Stat unit (RHI thread, in seconds)
	FrameTime_RHIThread,

	// Stat unit (inferred GPU time, in seconds)
	FrameTime_GPU,

	// Network ping (in ms)
	Ping,

	// The incoming packet loss percentage (%)
	PacketLoss_Incoming,

	// The outgoing packet loss percentage (%)
	PacketLoss_Outgoing,

	// The number of packets received in the last second
	PacketRate_Incoming,

	// The number of packets sent in the past second
	PacketRate_Outgoing,

	// The avg. size (in bytes) of packets received
	PacketSize_Incoming,

	// The avg. size (in bytes) of packets sent
	PacketSize_Outgoing,

	// The total latency in MS of the game
	Latency_Total,

	// Game simulation start to driver submission end
	Latency_Game,

	// OS render queue start to GPU render end
	Latency_Render,

	// New stats should go above here
	Count UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(ERsDisplayablePerformanceStat, ERsDisplayablePerformanceStat::Count);

// Describes one platform-specific device profile variant that the user can choose from in the UI
USTRUCT()
struct FRsQualityDeviceProfileVariant
{
	GENERATED_BODY()

	// The display name for this device profile variant (visible in the options screen)
	UPROPERTY(EditAnywhere)
	FText DisplayName;

	// The suffix to append to the base device profile name for the current platform
	UPROPERTY(EditAnywhere)
	FString DeviceProfileSuffix;

	// The minimum required refresh rate to enable this mode
	// (e.g., if this is set to 120 Hz and the device is connected
	// to a 60 Hz display, it won't be available)
	UPROPERTY(EditAnywhere)
	int32 MinRefreshRate = 0;
};

// Describes a set of performance stats that the user can enable in settings,
// predicated on passing a visibility query on platform traits
USTRUCT()
struct FRsPerformanceStatGroup
{
	GENERATED_BODY()

	// A query on platform traits to determine whether or not it will be possible
	// to show a set of stats
	UPROPERTY(EditAnywhere, meta=(Categories = "Input,Platform.Trait"))
	FGameplayTagQuery VisibilityQuery;

	// The set of stats to allow if the query passes
	UPROPERTY(EditAnywhere)
	TSet<ERsDisplayablePerformanceStat> AllowedStats;
};

// How hare frame pacing and overall graphics settings controlled/exposed for the platform?
UENUM()
enum class ERsFramePacingMode : uint8
{
	// Manual frame rate limits, user is allowed to choose whether or not to lock to vsync
	DesktopStyle,

	// Limits handled by choosing present intervals driven by device profiles
	ConsoleStyle,

	// Limits handled by a user-facing choice of frame rate from among ones allowed by device profiles for the specific device
	MobileStyle
};

UCLASS(config=Game, defaultconfig)
class URsPlatformSpecificRenderingSettings : public UPlatformSettings
{
	GENERATED_BODY()

public:
	URsPlatformSpecificRenderingSettings();

	// Helper method to get the performance settings object, directed via platform settings
	static const URsPlatformSpecificRenderingSettings* Get();

public:
	// The default variant suffix to append, should typically be a member of
	// UserFacingDeviceProfileOptions unless there is only one for the current platform
	//
	// Note that this will usually be set from platform-specific ini files, not via the UI
	UPROPERTY(EditAnywhere, Config, Category=DeviceProfiles)
	FString DefaultDeviceProfileSuffix;

	// The list of device profile variations to allow users to choose from in settings
	//
	// These should be sorted from slowest to fastest by target frame rate:
	//   If the current display doesn't support a user chosen refresh rate, we'll try
	//   previous entries until we find one that works
	//
	// Note that this will usually be set from platform-specific ini files, not via the UI
	UPROPERTY(EditAnywhere, Config, Category=DeviceProfiles)
	TArray<FRsQualityDeviceProfileVariant> UserFacingDeviceProfileOptions;

	// Does the platform support granular video quality settings?
	UPROPERTY(EditAnywhere, Config, Category=VideoSettings)
	bool bSupportsGranularVideoQualitySettings = true;

	// Does the platform support running the automatic quality benchmark (typically this should only be true if bSupportsGranularVideoQualitySettings is also true)
	UPROPERTY(EditAnywhere, Config, Category=VideoSettings)
	bool bSupportsAutomaticVideoQualityBenchmark = true;

	// How is frame pacing controlled
	UPROPERTY(EditAnywhere, Config, Category=VideoSettings)
	ERsFramePacingMode FramePacingMode = ERsFramePacingMode::DesktopStyle;

	// Potential frame rates to display for mobile
	// Note: This is further limited by Rs.DeviceProfile.Mobile.MaxFrameRate from the
	// platform-specific device profile and what the platform frame pacer reports as supported
	UPROPERTY(EditAnywhere, Config, Category=VideoSettings, meta=(EditCondition="FramePacingMode==ERsFramePacingMode::MobileStyle", ForceUnits=Hz))
	TArray<int32> MobileFrameRateLimits;
};

//////////////////////////////////////////////////////////////////////

/**
 * Project-specific performance profile settings.
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Rs Performance Settings"))
class URsPerformanceSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	URsPerformanceSettings();

private:
	// This is a special helper to expose the per-platform settings so they can be edited in the project settings
	// It never needs to be directly accessed
	UPROPERTY(EditAnywhere, Category = "PlatformSpecific")
	FPerPlatformSettings PerPlatformSettings;

public:
	// The list of frame rates to allow users to choose from in the various
	// "frame rate limit" video settings on desktop platforms
	UPROPERTY(EditAnywhere, Config, Category=Performance, meta=(ForceUnits=Hz))
	TArray<int32> DesktopFrameRateLimits;

	// The list of performance stats that can be enabled in Options by the user
	UPROPERTY(EditAnywhere, Config, Category=Stats)
	TArray<FRsPerformanceStatGroup> UserFacingPerformanceStats;
};
