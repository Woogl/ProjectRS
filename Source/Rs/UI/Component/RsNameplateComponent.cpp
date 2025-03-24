// Copyright 2024 Team BH.


#include "RsNameplateComponent.h"

#include "Extensions/UserWidgetExtension.h"
#include "Rs/Character/RsCharacterBase.h"
#include "Rs/UI/ViewModel/RsCharacterViewModel.h"
#include "View/MVVMView.h"

URsNameplateComponent::URsNameplateComponent()
{
	Space = EWidgetSpace::Screen;
	bDrawAtDesiredSize = true;
}

void URsNameplateComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetWidget())
	{
		if (ARsCharacterBase* OwnerCharacter = Cast<ARsCharacterBase>(GetOwner()))
		{
			if (URsCharacterViewModel* CharacterViewModel = URsCharacterViewModel::CreateRsCharacterViewModel(OwnerCharacter))
			{
				if (UMVVMView* MVVM = Cast<UMVVMView>(GetWidget()->GetExtension(UMVVMView::StaticClass())))
				{
					MVVM->SetViewModelByClass(CharacterViewModel);
				}
			}
		}
	}
}
