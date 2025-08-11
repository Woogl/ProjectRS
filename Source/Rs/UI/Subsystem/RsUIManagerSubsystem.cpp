// Copyright 2024 Team BH.


#include "RsUIManagerSubsystem.h"

#include "CommonActivatableWidget.h"
#include "CommonLocalPlayer.h"
#include "CommonUIExtensions.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Party/RsPartyLibrary.h"
#include "Rs/System/RsGameSetting.h"
#include "Rs/UI/RsUILibrary.h"
#include "Rs/UI/ViewModel/RsBattleViewModel.h"
#include "Rs/UI/ViewModel/RsPartyViewModel.h"
#include "Rs/UI/ViewModel/RsPlayerCharacterViewModel.h"
#include "Rs/UI/Widget/RsHUDLayout.h"
#include "View/MVVMView.h"

void URsUIManagerSubsystem::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	Super::NotifyPlayerAdded(LocalPlayer);

	if (LocalPlayer)
	{
		LocalPlayer->CallAndRegister_OnPlayerPawnSet(UCommonLocalPlayer::FPlayerPawnSetDelegate::FDelegate::CreateUObject(this, &ThisClass::RegisterGameHUD));

		// Bind only first time.
		if (!LinkSkillReadyHandle.IsValid())
		{
			if (URsBattleSubsystem* BattleSubsystem = LocalPlayer->GetSubsystem<URsBattleSubsystem>())
			{
				LinkSkillReadyHandle = BattleSubsystem->OnLinkSkillReady.AddUObject(this, &ThisClass::HandleLinkSkillReady);
			}
		}
	}
}

URsHUDLayout* URsUIManagerSubsystem::GetGameHUD() const
{
	return RsHUDInstance;
}

void URsUIManagerSubsystem::RegisterGameHUD(UCommonLocalPlayer* LocalPlayer, APawn* Pawn)
{
	if (LocalPlayer == nullptr || Pawn == nullptr)
	{
		return;
	}

	// Prevent crash on LV_Title and LV_Lobby.
	if (!Pawn->IsA(ARsPlayerCharacter::StaticClass()))
	{
		return;
	}
	
	if (URsGameSetting::Get()->GameHUDClass == nullptr)
	{
		UE_LOG(RsLog, Error, TEXT("Game HUD Class is null! See DA_GameSetting!"));
		//UE_LOG(RsLog, Error, TEXT("Game HUD Class is null! See DefaultGame.ini!"));
		return;
	}

	// Create Game HUD instance first only.
	if (RsHUDInstance == nullptr)
	{
		UCommonActivatableWidget* HUDInstance = UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, RsGameplayTags::UI_LAYER_GAME, URsGameSetting::Get()->GameHUDClass);
		RsHUDInstance = Cast<URsHUDLayout>(HUDInstance);
	}

	// Set the view model to Game HUD instance.
	if (RsHUDInstance)
	{
		if (ARsPlayerCharacter* OwnerCharacter = Cast<ARsPlayerCharacter>(Pawn))
		{
			if (URsPlayerCharacterViewModel* PCViewModel = URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(OwnerCharacter))
			{
				if (UMVVMView* View = Cast<UMVVMView>(RsHUDInstance->GetExtension<UMVVMView>()))
				{
					View->SetViewModelByClass(PCViewModel);
				}
			}
		}
	}
}

void URsUIManagerSubsystem::HandleLinkSkillReady(ARsEnemyCharacter* Target, ERsLinkSkillType Type, int32 AvailableCount)
{
	ULocalPlayer* LocalPlayer = GetGameInstance()->GetFirstGamePlayer();
	if (!LocalPlayer)
	{
		return;
	}

	if (!Target || AvailableCount == 0 || URsPartyLibrary::GetAlivePartyMemberCount(LocalPlayer) <= 1)
	{
		return;
	}
	
	if (UCommonActivatableWidget* TripleLinkSkillWidget = UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, RsGameplayTags::UI_LAYER_GAME, URsGameSetting::Get()->TripleLinkSkillWidget))
	{
		if (URsBattleSubsystem* BattleSubsystem = LocalPlayer->GetSubsystem<URsBattleSubsystem>())
		{
			if (URsBattleViewModel* BattleViewModel = URsBattleViewModel::GetRsBattleViewModel(BattleSubsystem))
			{
				URsUILibrary::SetViewModelByClass(TripleLinkSkillWidget, BattleViewModel);
			}
			if (URsPartyViewModel* PartyViewModel = URsPartyViewModel::CreateRsPartyViewModel(URsPartyLibrary::GetPartyComponent(this)))
			{
				URsUILibrary::SetViewModelByClass(TripleLinkSkillWidget, PartyViewModel);
			}
		}
	}
}

