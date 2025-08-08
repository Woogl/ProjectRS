// Copyright 2025 Team BH.


#include "RsBattleSubsystem.h"

#include "CommonUIExtensions.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/Character/RsEnemyCharacter.h"
#include "Rs/Party/RsPartyLibrary.h"
#include "Rs/System/RsGameSetting.h"
#include "Rs/UI/RsUILibrary.h"
#include "Rs/UI/Subsystem/RsMVVMGameSubsystem.h"
#include "Rs/UI/ViewModel/RsBattleViewModel.h"
#include "Rs/UI/ViewModel/RsPartyViewModel.h"
#include "Rs/UI/Widget/RsActivatableWidget.h"

void URsBattleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	URsMVVMGameSubsystem::GetOrCreateSingletonViewModel<URsBattleViewModel>(this);
}

ARsEnemyCharacter* URsBattleSubsystem::GetBossInBattle() const
{
	return BossInBattle.Get();
}

void URsBattleSubsystem::SetBossInBattle(ARsEnemyCharacter* Boss)
{
	BossInBattle = Boss;
	OnBossFight.Broadcast(Boss);
}

ARsEnemyCharacter* URsBattleSubsystem::GetLinkSkillTarget() const
{
	return LinkSkillTarget.Get();
}

void URsBattleSubsystem::SetLinkSkillTarget(ARsEnemyCharacter* Enemy, ERsLinkSkillType LinkSkillType)
{
	LinkSkillTarget = Enemy;
	if (LinkSkillTarget.IsValid() && LinkSkillType != ERsLinkSkillType::None && URsPartyLibrary::GetAlivePartyMemberCount(this) > 1)
	{
		if (LinkSkillType == ERsLinkSkillType::Parry)
		{
			AvailableLinkSkillCount = 1;
		}
		else if (LinkSkillType == ERsLinkSkillType::Triple)
		{
			AvailableLinkSkillCount = 3;
		}
		
		if (URsGameSetting::Get()->TripleLinkSkillWidget != nullptr && AvailableLinkSkillCount > 0)
		{
			ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			TSubclassOf<URsActivatableWidget> WidgetClass = TSubclassOf<URsActivatableWidget>(URsGameSetting::Get()->TripleLinkSkillWidget);
			TArray<URsViewModelBase*> ViewModels;
			ViewModels.Add(URsPartyViewModel::CreateRsPartyViewModel(URsPartyLibrary::GetPartyComponent(this)));
			ViewModels.Add(URsBattleViewModel::GetRsBattleViewModel(this));
			if (URsActivatableWidget* TripleLinkSkillWidget = URsUILibrary::PushSceneWidgetToLayer(LocalPlayer, RsGameplayTags::UI_LAYER_GAME, WidgetClass, ViewModels))
			{
				TripleLinkSkillWidget->OnDeactivated().AddUObject(this, &ThisClass::ResetLinkSkillTarget);
			}
		}
	}
	else
	{
		AvailableLinkSkillCount = 0;
	}
	
	OnLinkSkillReady.Broadcast(LinkSkillTarget.Get(), LinkSkillType, AvailableLinkSkillCount);
}

void URsBattleSubsystem::RemoveLinkSkillTarget(ARsEnemyCharacter* Enemy)
{
	if (!Enemy || !LinkSkillTarget.IsValid())
	{
		return;
	}
	
	if (LinkSkillTarget == Enemy)
	{
		LinkSkillTarget.Reset();
		AvailableLinkSkillCount = 0;

		OnLinkSkillReady.Broadcast(nullptr, ERsLinkSkillType::None, 0);
	}
}

void URsBattleSubsystem::ResetLinkSkillTarget()
{
	LinkSkillTarget.Reset();
	AvailableLinkSkillCount = 0;

	OnLinkSkillReady.Broadcast(nullptr, ERsLinkSkillType::None, 0);
}

bool URsBattleSubsystem::IsLinkSkillReady() const
{
	return AvailableLinkSkillCount > 0 && LinkSkillTarget.IsValid();
}

void URsBattleSubsystem::DecrementLinkSkillCount()
{
	AvailableLinkSkillCount = FMath::Max(AvailableLinkSkillCount - 1, 0);
	if (AvailableLinkSkillCount == 0)
	{
		OnLinkSkillReady.Broadcast(LinkSkillTarget.Get(), ERsLinkSkillType::None, 0);
	}
}
