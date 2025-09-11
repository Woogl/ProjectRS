// Copyright 2024 Team BH.


#include "RsGameSettingDataAsset.h"

#include "RsDeveloperSetting.h"

const URsGameSettingDataAsset* URsGameSettingDataAsset::Get()
{
	return URsDeveloperSetting::Get().GameSettingDataAsset.LoadSynchronous();
}
