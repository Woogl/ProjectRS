// Copyright 2024 Team BH.


#include "RsGameInstance.h"

#include "CommonLocalPlayer.h"
#include "RsGameSetting.h"
#include "Engine/AssetManager.h"
#include "Rs/UI/MVVM/RsMVVMGameSubsystem.h"

void URsGameInstance::Init()
{
	Super::Init();
	
	// Get RsGameSetting asset ID
	UAssetManager& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> OutAssets;
	AssetManager.GetPrimaryAssetIdList(TEXT("RsGameSetting"), OutAssets);
	
	if (OutAssets.Num() == 0)
	{
		FText Message = FText::FromString(TEXT("Can't find RsGameSetting data asset!"));
		FMessageDialog::Open(EAppMsgType::Ok, Message);
		return;
	}
	if (OutAssets.Num() > 1)
	{
		FText Message = FText::FromString(TEXT("There are many RsGameSetting data assets! There should be only 1!"));
		FMessageDialog::Open(EAppMsgType::Ok, Message);
		return;
	}

	// Load DA_RsGameSetting
	const FPrimaryAssetId& AssetId = OutAssets[0];
	const FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(AssetId);
	TSoftObjectPtr<URsGameSetting> GameSettingPtr(AssetPath);
	RsGameSetting = GameSettingPtr.LoadSynchronous();
}

int32 URsGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	GetSubsystem<URsMVVMGameSubsystem>()->NotifyPlayerAdded(Cast<UCommonLocalPlayer>(NewPlayer));
	
	return Super::AddLocalPlayer(NewPlayer, UserId);
}

const URsGameSetting* URsGameInstance::GetRsGameSetting() const
{
	return RsGameSetting;
}
