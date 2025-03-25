// Copyright 2024 Team BH.


#include "RsDeveloperSetting.h"

const URsDeveloperSetting* URsDeveloperSetting::Get()
{
	return GetDefault<URsDeveloperSetting>();
}

FName URsDeveloperSetting::GetCategoryName() const
{
	return FApp::GetProjectName();
}
