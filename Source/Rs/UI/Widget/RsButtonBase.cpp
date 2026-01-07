// Copyright 2025 Team BH.


#include "RsButtonBase.h"

#include "CommonActionWidget.h"
#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Rs/UI/RsUIManagerSubsystem.h"

void URsButtonBase::SetButtonText(const FText& InText)
{
	if (Text_ButtonText && !InText.IsEmpty())
	{
		Text_ButtonText->SetText(InText);
	}
}

void URsButtonBase::SetButtionImage(const FSlateBrush& InBrush)
{
	if (LazyImage_ButtonImage)
	{
		LazyImage_ButtonImage->SetBrush(InBrush);
	}
}

void URsButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetButtonText(ButtonText);
}

void URsButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();
	
	if (Text_ButtonText && GetCurrentTextStyleClass())
	{
		Text_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

void URsButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();
	
	if (!DescriptionText.IsEmpty())
	{
		if (URsUIManagerSubsystem* Subsystem = URsUIManagerSubsystem::Get(this))
		{
			Subsystem->OnButtonDescriptionTextUpdated.Broadcast(this, DescriptionText);
		}
	}
}

void URsButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	
	if (URsUIManagerSubsystem* Subsystem = URsUIManagerSubsystem::Get(this))
	{
		Subsystem->OnButtonDescriptionTextUpdated.Broadcast(this, DescriptionText);
	}
}

// void URsButtonBase::RefreshButtonText()
// {
// 	if (ButtonText.IsEmpty())
// 	{
// 		if (InputActionWidget)
// 		{
// 			const FText ActionDisplayText = InputActionWidget->GetDisplayText();	
// 			if (!ActionDisplayText.IsEmpty())
// 			{
// 				UpdateButtonText(ActionDisplayText);
// 				return;
// 			}
// 		}
// 	}
// 	
// 	UpdateButtonText(ButtonText);	
// }
