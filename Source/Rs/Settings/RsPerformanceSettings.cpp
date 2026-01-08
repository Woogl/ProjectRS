// Copyright Epic Games, Inc. All Rights Reserved.

#include "RsPerformanceSettings.h"

#include "Engine/PlatformSettingsManager.h"
#include "Misc/EnumRange.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RsPerformanceSettings)

//////////////////////////////////////////////////////////////////////

URsPlatformSpecificRenderingSettings::URsPlatformSpecificRenderingSettings()
{
	MobileFrameRateLimits.Append({ 20, 30, 45, 60, 90, 120 });
}

const URsPlatformSpecificRenderingSettings* URsPlatformSpecificRenderingSettings::Get()
{
	URsPlatformSpecificRenderingSettings* Result = UPlatformSettingsManager::Get().GetSettingsForPlatform<ThisClass>();
	check(Result);
	return Result;
}

//////////////////////////////////////////////////////////////////////

URsPerformanceSettings::URsPerformanceSettings()
{
	PerPlatformSettings.Initialize(URsPlatformSpecificRenderingSettings::StaticClass());

	CategoryName = TEXT("Game");

	DesktopFrameRateLimits.Append({ 30, 60, 120, 144, 160, 165, 180, 200, 240, 360 });

	// Default to all stats are allowed
	FRsPerformanceStatGroup& StatGroup = UserFacingPerformanceStats.AddDefaulted_GetRef();
	for (ERsDisplayablePerformanceStat PerfStat : TEnumRange<ERsDisplayablePerformanceStat>())
	{
		StatGroup.AllowedStats.Add(PerfStat);
	}
}

