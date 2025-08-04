// Copyright 2025 Team BH.


#include "RsBattleViewModel.h"

#include "RsCharacterViewModel.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/Character/RsEnemyCharacter.h"

URsBattleViewModel* URsBattleViewModel::CreateRsBattleViewModel(URsBattleSubsystem* BattleSubsystem)
{
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

bool URsBattleViewModel::GetIsLinkSkillReady() const
{
	if (URsBattleSubsystem* Model = GetModel<URsBattleSubsystem>())
	{
		return Model->IsLinkSkillReady();
	}
	return false;
}

void URsBattleViewModel::HandleBossFight(ARsEnemyCharacter* Boss)
{
	BossViewModel = URsCharacterViewModel::CreateRsCharacterViewModel(Boss);
	SetBossViewModel(BossViewModel);
}

void URsBattleViewModel::HandleLinkSkillReady(ARsEnemyCharacter* LinkSkillTarget, ERsLinkSkillType LinkSkillType, int32 LinkSkillCount)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIsLinkSkillReady);
}

void URsBattleViewModel::SetBossViewModel(URsCharacterViewModel* InBossViewModel)
{
	UE_MVVM_SET_PROPERTY_VALUE(BossViewModel, InBossViewModel);
}
