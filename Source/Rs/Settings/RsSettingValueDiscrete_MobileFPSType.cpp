// Copyright Epic Games, Inc. All Rights Reserved.

#include "RsSettingValueDiscrete_MobileFPSType.h"

#include "RsPerformanceSettings.h"
#include "RsSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RsSettingValueDiscrete_MobileFPSType)

#define LOCTEXT_NAMESPACE "RsSettings"

URsSettingValueDiscrete_MobileFPSType::URsSettingValueDiscrete_MobileFPSType()
{
}

void URsSettingValueDiscrete_MobileFPSType::OnInitialized()
{
	Super::OnInitialized();

	const URsPlatformSpecificRenderingSettings* PlatformSettings = URsPlatformSpecificRenderingSettings::Get();
	const URsSettingsLocal* UserSettings = URsSettingsLocal::Get();

	for (int32 TestLimit : PlatformSettings->MobileFrameRateLimits)
	{
		if (URsSettingsLocal::IsSupportedMobileFramePace(TestLimit))
		{
			FPSOptions.Add(TestLimit, MakeLimitString(TestLimit));
		}
	}

	const int32 FirstFrameRateWithQualityLimit = UserSettings->GetFirstFrameRateWithQualityLimit();
	if (FirstFrameRateWithQualityLimit > 0)
	{
		SetWarningRichText(FText::Format(LOCTEXT("MobileFPSType_Note", "<strong>Note: Changing the framerate setting to {0} or higher might lower your Quality Presets.</>"), MakeLimitString(FirstFrameRateWithQualityLimit)));
	}
}

int32 URsSettingValueDiscrete_MobileFPSType::GetDefaultFPS() const
{
	return URsSettingsLocal::GetDefaultMobileFrameRate();
}

FText URsSettingValueDiscrete_MobileFPSType::MakeLimitString(int32 Number)
{
	return FText::Format(LOCTEXT("MobileFrameRateOption", "{0} FPS"), FText::AsNumber(Number));
}

void URsSettingValueDiscrete_MobileFPSType::StoreInitial()
{
	InitialValue = GetValue();
}

void URsSettingValueDiscrete_MobileFPSType::ResetToDefault()
{
	SetValue(GetDefaultFPS(), EGameSettingChangeReason::ResetToDefault);
}

void URsSettingValueDiscrete_MobileFPSType::RestoreToInitial()
{
	SetValue(InitialValue, EGameSettingChangeReason::RestoreToInitial);
}

void URsSettingValueDiscrete_MobileFPSType::SetDiscreteOptionByIndex(int32 Index)
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);

	int32 NewMode = FPSOptionsModes.IsValidIndex(Index) ? FPSOptionsModes[Index] : GetDefaultFPS();

	SetValue(NewMode, EGameSettingChangeReason::Change);
}

int32 URsSettingValueDiscrete_MobileFPSType::GetDiscreteOptionIndex() const
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);
	return FPSOptionsModes.IndexOfByKey(GetValue());
}

TArray<FText> URsSettingValueDiscrete_MobileFPSType::GetDiscreteOptions() const
{
	TArray<FText> Options;
	FPSOptions.GenerateValueArray(Options);

	return Options;
}

int32 URsSettingValueDiscrete_MobileFPSType::GetValue() const
{
	return URsSettingsLocal::Get()->GetDesiredMobileFrameRateLimit();
}

void URsSettingValueDiscrete_MobileFPSType::SetValue(int32 NewLimitFPS, EGameSettingChangeReason InReason)
{
	URsSettingsLocal::Get()->SetDesiredMobileFrameRateLimit(NewLimitFPS);

	NotifySettingChanged(InReason);
}

#undef LOCTEXT_NAMESPACE

