// Copyright 2025 Team BH.


#include "RsBattleViewModel.h"

#include "RsCharacterViewModel.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/UI/MVVM/RsMVVMGameSubsystem.h"

URsBattleViewModel* URsBattleViewModel::GetBattleViewModel(UObject* WorldContext)
{
	return URsMVVMGameSubsystem::GetSingletonViewModel<URsBattleViewModel>(WorldContext);
}

void URsBattleViewModel::Initialize()
{
	Super::Initialize();

	if (URsBattleSubsystem* BattleSubsystem = GetModel<URsBattleSubsystem>())
	{
		BattleSubsystem->OnBossFight.AddUObject(this, &ThisClass::HandleBossFight);
		BattleSubsystem->OnLinkSkillReady.AddUObject(this, &ThisClass::HandleLinkSkillReady);
		BattleSubsystem->OnLinkSkillFinish.AddUObject(this, &ThisClass::HandleLinkSkillFinish);
	}
}

void URsBattleViewModel::Deinitialize()
{
	Super::Deinitialize();

	if (URsBattleSubsystem* BattleSubsystem = GetModel<URsBattleSubsystem>())
	{
		BattleSubsystem->OnBossFight.RemoveAll(this);
		BattleSubsystem->OnLinkSkillReady.RemoveAll(this);
		BattleSubsystem->OnLinkSkillFinish.RemoveAll(this);
	}
}

void URsBattleViewModel::DecrementLinkSkillCount(ARsCharacterBase* CurrentTarget, ERsLinkSkillType LinkSkillType)
{
	if (URsBattleSubsystem* BattleSubsystem = GetModel<URsBattleSubsystem>())
	{
		BattleSubsystem->DecrementLinkSkillCount(CurrentTarget, LinkSkillType);
	}
}

bool URsBattleViewModel::GetIsLinkSkillReady() const
{
	if (URsBattleSubsystem* BattleSubsystem = GetModel<URsBattleSubsystem>())
	{
		return BattleSubsystem->IsLinkSkillReady();
	}
	return false;
}

ARsCharacterBase* URsBattleViewModel::GetLinkSkillTarget() const
{
	if (URsBattleSubsystem* BattleSubsystem = GetModel<URsBattleSubsystem>())
	{
		return BattleSubsystem->GetLinkSkillTarget();
	}
	return nullptr;
}

void URsBattleViewModel::HandleBossFight(ARsCharacterBase* Boss)
{
	BossViewModel = URsCharacterViewModel::CreateRsCharacterViewModel(Boss);
	SetBossViewModel(BossViewModel);
}

void URsBattleViewModel::HandleLinkSkillReady(ARsCharacterBase* LinkSkillTarget, ERsLinkSkillType LinkSkillType, int32 LinkSkillCount)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIsLinkSkillReady);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLinkSkillTarget);
}

void URsBattleViewModel::HandleLinkSkillFinish(ERsLinkSkillType LinkSkillType)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIsLinkSkillReady);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLinkSkillTarget);
}

void URsBattleViewModel::SetBossViewModel(URsCharacterViewModel* InBossViewModel)
{
	UE_MVVM_SET_PROPERTY_VALUE(BossViewModel, InBossViewModel);
}
