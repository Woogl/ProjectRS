// Copyright 2025 Team BH.


#include "RsBattleViewModel.h"

#include "RsCharacterViewModel.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/Character/RsEnemyCharacter.h"

URsBattleViewModel* URsBattleViewModel::CreateRsBattleViewModel(UWorld* World)
{
	if (!World)
	{
		return nullptr;
	}
	URsBattleSubsystem* BattleSubsystem = World->GetSubsystem<URsBattleSubsystem>();
	if (!BattleSubsystem)
	{
		return nullptr;
	}
	if (URsBattleViewModel* ViewModel = NewObject<URsBattleViewModel>(BattleSubsystem))
	{
		ViewModel->Initialize();
	}
	return nullptr;
}

void URsBattleViewModel::Initialize()
{
	Super::Initialize();

	if (URsBattleSubsystem* Model = GetModel<URsBattleSubsystem>())
	{
		Model->OnBossFight.AddUObject(this, &ThisClass::HandleBossFight);
		Model->OnLinkSkillReady.AddUObject(this, &ThisClass::HandleLinkSkillReady);
	}
}

void URsBattleViewModel::Deinitialize()
{
	Super::Deinitialize();

	if (URsBattleSubsystem* Model = GetModel<URsBattleSubsystem>())
	{
		Model->OnBossFight.RemoveAll(this);
		Model->OnLinkSkillReady.RemoveAll(this);
	}
}

void URsBattleViewModel::HandleBossFight(ARsEnemyCharacter* Boss)
{
	BossViewModel = URsCharacterViewModel::CreateRsCharacterViewModel(Boss);
	SetBossViewModel(BossViewModel);
}

void URsBattleViewModel::HandleLinkSkillReady(ARsEnemyCharacter* LinkSkillTarget, ERsLinkSkillType LinkSkillType, int32 LinkSkillCount)
{
	if (URsBattleSubsystem* Model = GetModel<URsBattleSubsystem>())
	{
		UE_MVVM_SET_PROPERTY_VALUE(bIsLinkSkillReady, Model->IsLinkSkillReady());
	}
}

void URsBattleViewModel::SetBossViewModel(URsCharacterViewModel* InBossViewModel)
{
	UE_MVVM_SET_PROPERTY_VALUE(BossViewModel, InBossViewModel);
}
