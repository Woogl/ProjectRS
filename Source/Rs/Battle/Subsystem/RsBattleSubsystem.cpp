// Copyright 2025 Team BH.


#include "RsBattleSubsystem.h"

#include "Rs/RsGameplayTags.h"
#include "Rs/Character/RsEnemyCharacter.h"
#include "Rs/Party/RsPartyLibrary.h"
#include "Rs/System/RsGameSetting.h"
#include "Rs/UI/RsUILibrary.h"
#include "Rs/UI/ViewModel/RsBattleViewModel.h"
#include "Rs/UI/ViewModel/RsPartyViewModel.h"
#include "Rs/UI/Widget/RsActivatableWidget.h"

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
		AciveLinkSkillType = LinkSkillType;
		if (AciveLinkSkillType == ERsLinkSkillType::Parry)
		{
			LinkSkillActiveCount = 1;
		}
		else if (AciveLinkSkillType == ERsLinkSkillType::Triple)
		{
			LinkSkillActiveCount = 3;
		}
		
		if (URsGameSetting::Get()->TripleLinkSkillWidget != nullptr && LinkSkillActiveCount > 0)
		{
			float TimerDuration = URsGameSetting::Get()->TripleLinkSkillDuration;
			
			ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			TSubclassOf<URsActivatableWidget> WidgetClass = TSubclassOf<URsActivatableWidget>(URsGameSetting::Get()->TripleLinkSkillWidget);
			TArray<URsViewModelBase*> ViewModels;
			ViewModels.Add(URsPartyViewModel::CreateRsPartyViewModel(URsPartyLibrary::GetPartyComponent(this)));
			ViewModels.Add(URsBattleViewModel::CreateRsBattleViewModel(this));
			if (URsActivatableWidget* TripleLinkSkillWidget = URsUILibrary::PushSceneWidgetToLayer(LocalPlayer, RsGameplayTags::UI_LAYER_GAME, WidgetClass, ViewModels))
			{
				TimerDuration = TimerDuration * TripleLinkSkillWidget->TimeDilation;
			}
			GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, this, &ThisClass::ResetLinkSkillTarget, TimerDuration);
		}
	}
	else
	{
		LinkSkillActiveCount = 0;
		AciveLinkSkillType = ERsLinkSkillType::None;
	}
	
	OnLinkSkillReady.Broadcast(LinkSkillTarget.Get(), AciveLinkSkillType, LinkSkillActiveCount);
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
		LinkSkillActiveCount = 0;

		OnLinkSkillReady.Broadcast(nullptr, ERsLinkSkillType::None, 0);
	}
}

void URsBattleSubsystem::ResetLinkSkillTarget()
{
	LinkSkillTarget.Reset();
	LinkSkillActiveCount = 0;
	AciveLinkSkillType = ERsLinkSkillType::None;
	GetWorld()->GetTimerManager().ClearTimer(ResetTimerHandle);

	OnLinkSkillReady.Broadcast(nullptr, ERsLinkSkillType::None, 0);
}

bool URsBattleSubsystem::IsLinkSkillReady() const
{
	return LinkSkillActiveCount > 0 && LinkSkillTarget.IsValid();
}

void URsBattleSubsystem::DecrementLinkSkillCount()
{
	LinkSkillActiveCount = FMath::Max(LinkSkillActiveCount - 1, 0);
	if (LinkSkillActiveCount == 0)
	{
		OnLinkSkillReady.Broadcast(LinkSkillTarget.Get(), ERsLinkSkillType::None, 0);
	}
}
