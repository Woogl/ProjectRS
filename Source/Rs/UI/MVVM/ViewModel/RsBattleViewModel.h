// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsSingletonViewModelBase.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "RsBattleViewModel.generated.h"

class ARsCharacterBase;
class URsBattleSubsystem;
enum class ERsLinkSkillType : uint8;
class URsCharacterViewModel;
/**
 * 
 */
UCLASS()
class RS_API URsBattleViewModel : public URsSingletonViewModelBase
{
	GENERATED_BODY()

public:
	using ModelType = URsBattleSubsystem;
	
	// Singleton Viewmodel
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel", meta=(WorldContext="WorldContext"))
	static URsBattleViewModel* GetBattleViewModel(UObject* WorldContext);
	
	virtual void Initialize() override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	void DecrementLinkSkillCount(ARsCharacterBase* CurrentTarget, ERsLinkSkillType LinkSkillType);

public:
	UFUNCTION(FieldNotify, BlueprintPure)
	bool GetIsLinkSkillReady() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	ARsCharacterBase* GetLinkSkillTarget() const;
	
private:
	void HandleBossFight(ARsCharacterBase* Boss);
	void HandleLinkSkillReady(ARsCharacterBase* LinkSkillTarget, ERsLinkSkillType LinkSkillType, int32 LinkSkillCount);
	void HandleLinkSkillFinish(ERsLinkSkillType LinkSkillType);
	
	void SetBossViewModel(URsCharacterViewModel* Boss);

public:
	UPROPERTY(FieldNotify, Setter, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsCharacterViewModel> BossViewModel;
};
