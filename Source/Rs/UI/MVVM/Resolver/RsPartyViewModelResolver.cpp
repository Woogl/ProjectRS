// Copyright 2025 Team BH.


#include "RsPartyViewModelResolver.h"

#include "Blueprint/UserWidget.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/Party/RsPartyLibrary.h"
#include "Rs/UI/MVVM/RsMVVMGameSubsystem.h"
#include "Rs/UI/MVVM/ViewModel/RsPartyViewModel.h"

UObject* URsPartyViewModelResolver::CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget, const UMVVMView* View) const
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
	
	if (URsPartyViewModel* ExistingViewModel = Subsystem->GetSingletonViewModel<URsPartyViewModel>(LocalPlayer))
	{
		return ExistingViewModel;
	}

	URsPartyComponent* PartyComponent = URsPartyLibrary::GetPartyComponent(LocalPlayer);
	if (!PartyComponent)
	{
		return nullptr;
	}
	
	return Subsystem->CreateSingletonViewModel<URsPartyViewModel>(PartyComponent);
}
