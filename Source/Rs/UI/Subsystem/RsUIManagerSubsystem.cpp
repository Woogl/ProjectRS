// Copyright 2024 Team BH.


#include "RsUIManagerSubsystem.h"

#include "CommonActivatableWidget.h"
#include "CommonLocalPlayer.h"
#include "CommonUIExtensions.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/System/RsGameSetting.h"
#include "Rs/UI/ViewModel/RsPlayerCharacterViewModel.h"
#include "View/MVVMView.h"

void URsUIManagerSubsystem::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	Super::NotifyPlayerAdded(LocalPlayer);

	if (LocalPlayer)
	{
		LocalPlayer->CallAndRegister_OnPlayerPawnSet(UCommonLocalPlayer::FPlayerPawnSetDelegate::FDelegate::CreateUObject(this, &ThisClass::RegisterGameHUD));
	}
}

void URsUIManagerSubsystem::RegisterGameHUD(UCommonLocalPlayer* LocalPlayer, APawn* Pawn)
{
	if (LocalPlayer == nullptr || Pawn == nullptr)
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
	if (GameHUDInstance == nullptr)
	{
		GameHUDInstance = UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, RsGameplayTags::TAG_UI_LAYER_GAME, URsGameSetting::Get()->GameHUDClass);
	}

	// Set the view model to Game HUD instance.
	if (GameHUDInstance)
	{
		if (ARsPlayerCharacter* OwnerCharacter = Cast<ARsPlayerCharacter>(Pawn))
		{
			if (URsCharacterViewModel* PCViewModel = URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(OwnerCharacter))
			{
				if (UMVVMView* MVVM = Cast<UMVVMView>(GameHUDInstance->GetExtension(UMVVMView::StaticClass())))
				{
					MVVM->SetViewModelByClass(PCViewModel);
				}
			}
		}
	}
}

