// Copyright 2024 Team BH.


#include "RsGameSetting.h"

#include "Engine/AssetManager.h"

FPrimaryAssetId URsGameSetting::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("RsGameSetting"));
}

URsGameSetting* URsGameSetting::Get()
{
	const FPrimaryAssetId AssetId = FPrimaryAssetId(TEXT("RsGameSetting"));
	UAssetManager& AssetManager = UAssetManager::Get();
	return Cast<URsGameSetting>(AssetManager.GetPrimaryAssetObject(AssetId));
}
