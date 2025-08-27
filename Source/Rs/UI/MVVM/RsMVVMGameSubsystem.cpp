// Copyright 2025 Team BH.


#include "RsMVVMGameSubsystem.h"

#include "CommonLocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/UI/MVVM/ViewModel/RsBattleViewModel.h"
#include "Rs/UI/MVVM/ViewModel/RsPartyViewModel.h"

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
	if (!LocalPlayer)
	{
		return;
	}

	LocalPlayer->CallAndRegister_OnPlayerControllerSet(UCommonLocalPlayer::FPlayerControllerSetDelegate::FDelegate::CreateUObject(this, &ThisClass::CreateSingletonViewModels_PlayerController));
}

void URsMVVMGameSubsystem::CreateSingletonViewModels_PlayerController(UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
{
	if (!LocalPlayer || !PlayerController || !PlayerController->IsLocalController())
	{
		return;
	}
	
	if (URsBattleSubsystem* BattleSubsystem = LocalPlayer->GetSubsystem<URsBattleSubsystem>())
	{
		CreateSingletonViewModel<URsBattleViewModel>(BattleSubsystem);
	}

	if (URsPartyComponent* PartyComponent = PlayerController->FindComponentByClass<URsPartyComponent>())
	{
		CreateSingletonViewModel<URsPartyViewModel>(PartyComponent);
	}
}
