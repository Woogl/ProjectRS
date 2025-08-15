// Copyright 2024 Team BH.


#include "RsUIManagerSubsystem.h"

#include "CommonActivatableWidget.h"
#include "CommonLocalPlayer.h"
#include "CommonUIExtensions.h"
#include "PrimaryGameLayout.h"
#include "RsMVVMGameSubsystem.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Party/RsPartyLibrary.h"
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
		UE_LOG(RsLog, Error, TEXT("GameHUD is null! See RsDeveloperSetting!"));
		return;
	}

	// Create Game HUD instance first only.
	if (!RsHUDInstance)
	{
		UClass* WidgetClassLoaded = GameHUDClass.LoadSynchronous();
		UCommonActivatableWidget* HUDInstance = UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, RsGameplayTags::UI_LAYER_GAME, WidgetClassLoaded);
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

	UClass* TripleLinkSkillWidgetClass = GetDefault<URsUIManagerSettings>()->TripleLinkSkillWidgetClass.Get();
	if (TripleLinkSkillWidgetClass)
	{
		URsBattleViewModel* BattleViewModel = URsMVVMGameSubsystem::GetSingletonViewModel<URsBattleViewModel>(Target);
		//URsPartyViewModel* PartyViewModel = URsMVVMGameSubsystem::GetSingletonViewModel<URsPartyViewModel>(Target);
		URsPartyViewModel* PartyViewModel = URsPartyViewModel::CreateRsPartyViewModel(URsPartyLibrary::GetPartyComponent(this));
		URsUILibrary::PushSceneWidgetToLayerAsync(LocalPlayer, RsGameplayTags::UI_LAYER_GAME, true, TripleLinkSkillWidgetClass, { BattleViewModel, PartyViewModel });
	}
}

FName URsUIManagerSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}

