// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsViewModelBase.h"
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
class RS_API URsBattleViewModel : public URsViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsBattleViewModel* GetRsBattleViewModel(URsBattleSubsystem* BattleSubsystem);
	
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
