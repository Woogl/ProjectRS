// Copyright 2025 Team BH.


#include "RsAbilitySystemSettings.h"

const URsAbilitySystemSettings& URsAbilitySystemSettings::Get()
{
	return *GetDefault<URsAbilitySystemSettings>();
}

FName URsAbilitySystemSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}
