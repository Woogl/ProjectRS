// Copyright 2024 Team BH.


#include "RsGameInstance.h"

#include "RsGameSetting.h"
#include "Engine/AssetManager.h"

void URsGameInstance::Init()
{
	Super::Init();
	
	// Get RsGameSetting asset ID
	UAssetManager& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> OutAssets;
	AssetManager.GetPrimaryAssetIdList(TEXT("RsGameSetting"), OutAssets);
	if (OutAssets.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find RsGameSetting data asset."));
		return;
	}

	// Load DA_RsGameSetting
	FSoftObjectPath SoftObjectPath = AssetManager.GetPrimaryAssetPath(OutAssets[0]);
	FSoftObjectPtr AssetPtr(AssetManager.GetPrimaryAssetPath(OutAssets[0]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}
	RsGameSetting = Cast<URsGameSetting>(AssetPtr.Get());
}
