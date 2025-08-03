// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsViewModelBase.h"
#include "RsBattleViewModel.generated.h"

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
	static URsBattleViewModel* CreateRsBattleViewModel(UWorld* World);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

	void HandleBossFight(ARsEnemyCharacter* Boss);
	void HandleLinkSkillReady(ARsEnemyCharacter* LinkSkillTarget, ERsLinkSkillType LinkSkillType, int32 LinkSkillCount);
	
private:
	void SetBossViewModel(URsCharacterViewModel* Boss);

public:
	UPROPERTY(FieldNotify, Setter, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsCharacterViewModel> BossViewModel;

	UPROPERTY(FieldNotify)
	bool bIsLinkSkillReady = false;
};
