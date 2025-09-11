// Copyright 2025 Team BH.


#include "RsBattleSettings.h"

const URsBattleSettings& URsBattleSettings::Get()
{
	return *GetDefault<URsBattleSettings>();
}

FName URsBattleSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}
