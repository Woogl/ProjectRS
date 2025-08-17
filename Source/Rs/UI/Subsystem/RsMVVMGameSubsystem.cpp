// Copyright 2025 Team BH.


#include "RsMVVMGameSubsystem.h"

#include "CommonLocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/Party/RsPartyLibrary.h"
#include "Rs/UI/ViewModel/RsBattleViewModel.h"
#include "Rs/UI/ViewModel/RsPartyViewModel.h"

URsMVVMGameSubsystem* URsMVVMGameSubsystem::Get(const UObject* WorldContext)
{
	if (!WorldContext)
	{
		return nullptr;
	}
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContext))
	{
		return GameInstance->GetSubsystem<URsMVVMGameSubsystem>();
	}
	return nullptr;
}

void URsMVVMGameSubsystem::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	if (LocalPlayer && !PlayerAddedDelegateHandle.IsValid())
	{
		PlayerAddedDelegateHandle = LocalPlayer->CallAndRegister_OnPlayerPawnSet(UCommonLocalPlayer::FPlayerPawnSetDelegate::FDelegate::CreateUObject(this, &ThisClass::CreateSingletonViewModels));
	}
}

void URsMVVMGameSubsystem::CreateSingletonViewModels(UCommonLocalPlayer* LocalPlayer, APawn* Pawn)
{
	if (LocalPlayer && !GetSingletonViewModel<URsBattleViewModel>(Pawn, false))
	{
		if (URsBattleSubsystem* BattleSubsystem = LocalPlayer->GetSubsystem<URsBattleSubsystem>())
		{
			CreateSingletonViewModel<URsBattleViewModel>(BattleSubsystem);
		}
	}

	if (Pawn && !GetSingletonViewModel<URsPartyViewModel>(LocalPlayer, false))
	{
		if (URsPartyComponent* PartyComponent = URsPartyLibrary::GetPartyComponent(Pawn))
		{
			CreateSingletonViewModel<URsPartyViewModel>(PartyComponent);
		}
	}
}
