// Copyright 2025 Team BH.


#include "RsBattleViewModel.h"

#include "RsCharacterViewModel.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/Character/RsEnemyCharacter.h"
#include "Rs/UI/Subsystem/RsMVVMGameSubsystem.h"

URsBattleViewModel* URsBattleViewModel::GetRsBattleViewModel(URsBattleSubsystem* BattleSubsystem)
{
	if (!BattleSubsystem)
	{
		return nullptr;
	}
	return URsMVVMGameSubsystem::GetOrCreateSingletonViewModel<URsBattleViewModel>(BattleSubsystem);
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

void URsBattleViewModel::DecrementLinkSkillCount()
{
	if (URsBattleSubsystem* Model = GetModel<URsBattleSubsystem>())
	{
		Model->DecrementLinkSkillCount();
	}
}

void URsBattleViewModel::RemoveLinkSkillTarget(ARsEnemyCharacter* OldTarget)
{
	if (URsBattleSubsystem* Model = GetModel<URsBattleSubsystem>())
	{
		Model->RemoveLinkSkillTarget(OldTarget);
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

ARsEnemyCharacter* URsBattleViewModel::GetLinkSkillTarget() const
{
	if (URsBattleSubsystem* Model = GetModel<URsBattleSubsystem>())
	{
		return Model->GetLinkSkillTarget();
	}
	return nullptr;
}

void URsBattleViewModel::HandleBossFight(ARsEnemyCharacter* Boss)
{
	BossViewModel = URsCharacterViewModel::CreateRsCharacterViewModel(Boss);
	SetBossViewModel(BossViewModel);
}

void URsBattleViewModel::HandleLinkSkillReady(ARsEnemyCharacter* LinkSkillTarget, ERsLinkSkillType LinkSkillType, int32 LinkSkillCount)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIsLinkSkillReady);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLinkSkillTarget);
}

void URsBattleViewModel::SetBossViewModel(URsCharacterViewModel* InBossViewModel)
{
	UE_MVVM_SET_PROPERTY_VALUE(BossViewModel, InBossViewModel);
}
