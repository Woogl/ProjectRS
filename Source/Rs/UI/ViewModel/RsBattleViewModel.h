// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsViewModelBase.h"
#include "RsBattleViewModel.generated.h"

class URsBattleSubsystem;
enum class ERsLinkSkillType : uint8;
class ARsEnemyCharacter;
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

public:
	UFUNCTION(FieldNotify, BlueprintPure)
	bool GetIsLinkSkillReady() const;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLinkSkillReady_ViewModel, int32, AvailableCount);
	UPROPERTY(BlueprintAssignable)
	FOnLinkSkillReady_ViewModel OnLinkSkillReady;
	
private:
	void HandleBossFight(ARsEnemyCharacter* Boss);
	void HandleLinkSkillReady(ARsEnemyCharacter* LinkSkillTarget, ERsLinkSkillType LinkSkillType, int32 LinkSkillCount);
	
	void SetBossViewModel(URsCharacterViewModel* Boss);

public:
	UPROPERTY(FieldNotify, Setter, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsCharacterViewModel> BossViewModel;
};
