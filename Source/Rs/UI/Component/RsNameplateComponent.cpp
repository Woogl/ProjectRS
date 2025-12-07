// Copyright 2024 Team BH.


#include "RsNameplateComponent.h"

#include "Extensions/UserWidgetExtension.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/UI/MVVM/ViewModel/RsCharacterViewModel.h"
#include "View/MVVMView.h"

URsNameplateComponent::URsNameplateComponent()
{
	Space = EWidgetSpace::Screen;
	bDrawAtDesiredSize = true;
	SetVisibility(false);
}

void URsNameplateComponent::Initialize(ARsCharacterBase* InOwnerCharacter)
{
	if (!GetWidget() || !InOwnerCharacter)
	{
		return;
	}
	
	if (URsCharacterViewModel* CharacterViewModel = URsCharacterViewModel::CreateRsCharacterViewModel(InOwnerCharacter))
	{
		if (UMVVMView* MVVM = Cast<UMVVMView>(GetWidget()->GetExtension(UMVVMView::StaticClass())))
		{
			MVVM->SetViewModelByClass(CharacterViewModel);
			SetVisibility(true);
		}
	}
}
