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
#include "Rs/UI/Widget/RsHUDLayout.h"
#include "View/MVVMView.h"

void URsUIManagerSubsystem::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	Super::NotifyPlayerAdded(LocalPlayer);

	if (LocalPlayer)
	{
		LocalPlayer->CallAndRegister_OnPlayerPawnSet(UCommonLocalPlayer::FPlayerPawnSetDelegate::FDelegate::CreateUObject(this, &ThisClass::RegisterGameHUD));
	}
}

URsHUDLayout* URsUIManagerSubsystem::GetGameHUD()
{
	return RsHUDInstance;
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

