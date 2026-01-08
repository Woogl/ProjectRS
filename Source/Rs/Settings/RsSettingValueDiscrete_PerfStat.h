// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameSettingValueDiscrete.h"

#include "RsSettingValueDiscrete_PerfStat.generated.h"

enum class ERsDisplayablePerformanceStat : uint8;
enum class ERsStatDisplayMode : uint8;

class UObject;

UCLASS()
class URsSettingValueDiscrete_PerfStat : public UGameSettingValueDiscrete
{
	GENERATED_BODY()
	
public:

	URsSettingValueDiscrete_PerfStat();

	void SetStat(ERsDisplayablePerformanceStat InStat);

	/** UGameSettingValue */
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	/** UGameSettingValueDiscrete */
	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	/** UGameSettingValue */
	virtual void OnInitialized() override;
	
	void AddMode(FText&& Label, ERsStatDisplayMode Mode);
protected:
	TArray<FText> Options;
	TArray<ERsStatDisplayMode> DisplayModes;

	ERsDisplayablePerformanceStat StatToDisplay;
	ERsStatDisplayMode InitialMode;
};
