// Copyright 2025 Team BH.


#include "RsBattleSubsystem.h"

#include "CommonUIExtensions.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/Character/RsEnemyCharacter.h"
#include "Rs/Party/RsPartyLibrary.h"
#include "Rs/System/RsGameSetting.h"
#include "Rs/UI/ViewModel/RsBattleViewModel.h"
#include "Rs/UI/ViewModel/RsPartyViewModel.h"
#include "Rs/UI/Widget/RsActivatableWidget.h"
#include "View/MVVMView.h"

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

void URsBattleSubsystem::SetLinkSkillTarget(ARsEnemyCharacter* Enemy, ERsLinkSkillType LinkSkillType, int32 ActiveCount)
{
	LinkSkillTarget = Enemy;
	if (LinkSkillTarget.IsValid())
	{
		LinkSkillActiveCount = ActiveCount;
		
		if (URsGameSetting::Get()->TripleLinkSkillWidget != nullptr && ActiveCount > 0)
		{
			if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
			{
				ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
				if (UCommonActivatableWidget* SceneWidget = UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, RsGameplayTags::UI_LAYER_GAME, URsGameSetting::Get()->TripleLinkSkillWidget))
				{
					if (UMVVMView* View = Cast<UMVVMView>(SceneWidget->GetExtension<UMVVMView>()))
					{
						if (URsPartyViewModel* PartyViewModel = URsPartyViewModel::CreateRsPartyViewModel(URsPartyLibrary::GetPartyComponent(this)))
						{
							View->SetViewModelByClass(PartyViewModel);
						}
						if (URsBattleViewModel* BattleViewModel = URsBattleViewModel::CreateRsBattleViewModel(this))
						{
							View->SetViewModelByClass(BattleViewModel);
						}
					}
				}
			}
		}
	}
	OnLinkSkillReady.Broadcast(Enemy, LinkSkillType, ActiveCount);
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
	}
}

bool URsBattleSubsystem::IsLinkSkillReady() const
{
	return LinkSkillActiveCount > 0 && LinkSkillTarget.IsValid();
}

void URsBattleSubsystem::DecrementLinkSkillCount()
{
	LinkSkillActiveCount = FMath::Max(LinkSkillActiveCount - 1, 0);
}
