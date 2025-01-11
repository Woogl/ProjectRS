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

	UObject* AssetObject = AssetManager.GetPrimaryAssetObject(AssetId);	
	return Cast<URsGameSetting>(AssetObject);
}
