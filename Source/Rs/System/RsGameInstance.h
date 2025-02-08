// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameInstance.h"
#include "RsGameInstance.generated.h"

class URsGameSetting;
/**
 * 
 */
UCLASS()
class RS_API URsGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	UPROPERTY()
	TObjectPtr<URsGameSetting> RsGameSetting;
};
