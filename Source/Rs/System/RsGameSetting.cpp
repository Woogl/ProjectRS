// Copyright 2024 Team BH.


#include "RsGameSetting.h"

#include "RsGameInstance.h"

FPrimaryAssetId URsGameSetting::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("RsGameSetting"), GetFName());
}

URsGameSetting* URsGameSetting::Get()
{
	if (GWorld)
	{
		if (URsGameInstance* RsGameInstance = GWorld->GetGameInstanceChecked<URsGameInstance>())
		{
			return RsGameInstance->RsGameSetting;
		}
	}
	return nullptr;
}
