// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameSettingValueDiscrete.h"

#include "RsSettingValueDiscrete_Language.generated.h"

#define UE_API RS_API

class FText;
class UObject;

UCLASS(MinimalAPI)
class URsSettingValueDiscrete_Language : public UGameSettingValueDiscrete
{
	GENERATED_BODY()
	
public:

	UE_API URsSettingValueDiscrete_Language();

	/** UGameSettingValue */
	UE_API virtual void StoreInitial() override;
	UE_API virtual void ResetToDefault() override;
	UE_API virtual void RestoreToInitial() override;

	/** UGameSettingValueDiscrete */
	UE_API virtual void SetDiscreteOptionByIndex(int32 Index) override;
	UE_API virtual int32 GetDiscreteOptionIndex() const override;
	UE_API virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	/** UGameSettingValue */
	UE_API virtual void OnInitialized() override;
	UE_API virtual void OnApply() override;

protected:
	TArray<FString> AvailableCultureNames;
};

#undef UE_API
