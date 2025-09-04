// Copyright 2024 Team BH.


#include "RsUIManagerSubsystem.h"

#include "CommonLocalPlayer.h"

#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Party/RsPartyLibrary.h"
#include "Rs/UI/RsUILibrary.h"
#include "Rs/UI/MVVM/RsMVVMGameSubsystem.h"
#include "Rs/UI/MVVM/ViewModel/RsBattleViewModel.h"
#include "Rs/UI/MVVM/ViewModel/RsPartyViewModel.h"
#include "Rs/UI/Widget/RsHUDLayout.h"

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
	if (!LocalPlayer || !Pawn)
	{
		return;
	}

	// Prevent crash on LV_Title and LV_Lobby.
	if (!Pawn->IsA(ARsPlayerCharacter::StaticClass()))
	{
		return;
	}

	TSoftClassPtr<URsActivatableWidget> GameHUDClass = GetDefault<URsUIManagerSettings>()->GameHUDClass;
	if (GameHUDClass.IsNull())
	{
		UE_LOG(RsLog, Error, TEXT("GameHUDClass is null! See RsUIManagerSetting!"));
		return;
	}

	// Create Game HUD instance first only.
	if (!RsHUDInstance)
	{
		UClass* LoadedHUD = GameHUDClass.LoadSynchronous();
		URsActivatableWidget* NewHUD = URsUILibrary::PushSceneWidgetToLayer(LocalPlayer, RsGameplayTags::UI_LAYER_GAME, LoadedHUD, TArray<URsViewModelBase*>());
		if (NewHUD)
		{
			RsHUDInstance = Cast<URsHUDLayout>(NewHUD);
		}
	}
}

void URsUIManagerSubsystem::HandleLinkSkillReady(ARsCharacterBase* Target, ERsLinkSkillType Type, int32 AvailableCount)
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

	if (Type == ERsLinkSkillType::Triple)
	{
		TSoftClassPtr<URsActivatableWidget> TripleLinkSkillWidgetClass = GetDefault<URsUIManagerSettings>()->TripleLinkSkillWidgetClass;
		if (!TripleLinkSkillWidgetClass.IsNull())
		{
			URsBattleViewModel* BattleViewModel = URsMVVMGameSubsystem::GetSingletonViewModel<URsBattleViewModel>(Target);
			URsPartyViewModel* PartyViewModel = URsMVVMGameSubsystem::GetSingletonViewModel<URsPartyViewModel>(Target);
			URsUILibrary::PushSceneWidgetToLayer(LocalPlayer, RsGameplayTags::UI_LAYER_GAME, TripleLinkSkillWidgetClass.LoadSynchronous(), { BattleViewModel, PartyViewModel });
		}
		else
		{
			UE_LOG(RsLog, Error, TEXT("TripleLinkSkillWidgetClass is null! See RsUIManagerSetting!"));
		}
	}
}

FName URsUIManagerSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}

