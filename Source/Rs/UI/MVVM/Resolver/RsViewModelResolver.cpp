// Copyright 2025 Team BH.


#include "RsViewModelResolver.h"

#include "Blueprint/UserWidget.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/Party/RsPartyLibrary.h"
#include "Rs/UI/MVVM/RsMVVMGameSubsystem.h"
#include "Rs/UI/MVVM/ViewModel/RsPartyViewModel.h"

UObject* URsViewModelPartyViewModelResolver::CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget, const UMVVMView* View) const
{
	ULocalPlayer* LocalPlayer = UserWidget->GetOwningLocalPlayer();
	if (!LocalPlayer)
	{
		return nullptr;
	}

	URsMVVMGameSubsystem* Subsystem = URsMVVMGameSubsystem::Get(LocalPlayer);
	if (!Subsystem)
	{
		return nullptr;
	}
	
	URsPartyViewModel* ExistingViewModel = Subsystem->GetSingletonViewModel<URsPartyViewModel>(LocalPlayer);
	if (!ExistingViewModel)
	{
		return nullptr;
	}

	URsPartyComponent* PartyComponent = URsPartyLibrary::GetPartyComponent(LocalPlayer);
	if (!PartyComponent)
	{
		return nullptr;
	}
	
	return Subsystem->CreateSingletonViewModel<URsPartyViewModel>(PartyComponent);
}
