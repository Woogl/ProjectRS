// Copyright 2025 Team BH.


#include "RsBattleViewModel.h"

#include "RsCharacterViewModel.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/UI/Subsystem/RsMVVMGameSubsystem.h"

URsBattleViewModel* URsBattleViewModel::GetRsBattleViewModel(UObject* WorldContext)
{
	return URsMVVMGameSubsystem::GetSingletonViewModel<URsBattleViewModel>(WorldContext);
}

void URsBattleViewModel::Initialize()
{
	Super::Initialize();

	if (URsBattleSubsystem* Model = GetModel<URsBattleSubsystem>())
	{
		Model->OnBossFight.AddUObject(this, &ThisClass::HandleBossFight);
		Model->OnLinkSkillReady.AddUObject(this, &ThisClass::HandleLinkSkillReady);
		Model->OnLinkSkillFinish.AddUObject(this, &ThisClass::HandleLinkSkillFinish);
	}
}

void URsBattleViewModel::Deinitialize()
{
	Super::Deinitialize();

	if (URsBattleSubsystem* Model = GetModel<URsBattleSubsystem>())
	{
		Model->OnBossFight.RemoveAll(this);
		Model->OnLinkSkillReady.RemoveAll(this);
		Model->OnLinkSkillFinish.RemoveAll(this);
	}
}

void URsBattleViewModel::DecrementLinkSkillCount(ARsCharacterBase* CurrentTarget, ERsLinkSkillType LinkSkillType)
{
	if (URsBattleSubsystem* Model = GetModel<URsBattleSubsystem>())
	{
		Model->DecrementLinkSkillCount(CurrentTarget, LinkSkillType);
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

ARsCharacterBase* URsBattleViewModel::GetLinkSkillTarget() const
{
	if (URsBattleSubsystem* Model = GetModel<URsBattleSubsystem>())
	{
		return Model->GetLinkSkillTarget();
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
