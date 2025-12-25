// Copyright 2025 Team BH.


#include "RsTargetingSettings.h"

const URsTargetingSettings& URsTargetingSettings::Get()
{
	return *GetDefault<URsTargetingSettings>();
}

FName URsTargetingSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}
