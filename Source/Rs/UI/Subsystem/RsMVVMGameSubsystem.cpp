// Copyright 2025 Team BH.


#include "RsMVVMGameSubsystem.h"

#include "CommonLocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/UI/ViewModel/RsBattleViewModel.h"
#include "Rs/UI/ViewModel/RsPartyViewModel.h"

void URsMVVMGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// URsGameInstance* RsGameInstance = CastChecked<URsGameInstance>(GetGameInstance());
	// RsGameInstance->AddLocalPlayer()
}

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
	LocalPlayer->CallAndRegister_OnPlayerPawnSet(UCommonLocalPlayer::FPlayerPawnSetDelegate::FDelegate::CreateUObject(this, &ThisClass::CreateSingletonViewModels_Pawn));
}

void URsMVVMGameSubsystem::CreateSingletonViewModels_PlayerController(UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
{
	if (!LocalPlayer || !PlayerController || !PlayerController->IsLocalController())
	{
		return;
	}
	if (!GetSingletonViewModel<URsPartyViewModel>(LocalPlayer, false))
	{
		if (URsPartyComponent* PartyComponent = PlayerController->FindComponentByClass<URsPartyComponent>())
		{
			CreateSingletonViewModel<URsPartyViewModel>(PartyComponent);
		}
	}
}

void URsMVVMGameSubsystem::CreateSingletonViewModels_Pawn(UCommonLocalPlayer* LocalPlayer, APawn* Pawn)
{
	if (!LocalPlayer || !Pawn)
	{
		return;
	}
	if (!GetSingletonViewModel<URsBattleViewModel>(Pawn, false))
	{
		if (URsBattleSubsystem* BattleSubsystem = LocalPlayer->GetSubsystem<URsBattleSubsystem>())
		{
			CreateSingletonViewModel<URsBattleViewModel>(BattleSubsystem);
		}
	}
}
