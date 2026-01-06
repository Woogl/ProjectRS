// Copyright 2025 Team BH.


#include "RsButton.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Rs/UI/RsUIManagerSubsystem.h"

void URsButton::SetButtonText(const FText& InText)
{
	if (Text_ButtonText && !InText.IsEmpty())
	{
		Text_ButtonText->SetText(InText);
	}
}

FText URsButton::GetButtonText() const
{
	if (Text_ButtonText)
	{
		return Text_ButtonText->GetText();
	}

	return FText::GetEmpty();
}

void URsButton::SetButtionImage(const FSlateBrush& InBrush)
{
	if (LazyImage_ButtonImage)
	{
		LazyImage_ButtonImage->SetBrush(InBrush);
	}
}

void URsButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetButtonText(ButtonDisplayText);
}

void URsButton::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();
	
	if (Text_ButtonText && GetCurrentTextStyleClass())
	{
		Text_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

void URsButton::NativeOnHovered()
{
	Super::NativeOnHovered();
	
	if (!ButtonDescriptionText.IsEmpty())
	{
		if (URsUIManagerSubsystem* Subsystem = URsUIManagerSubsystem::Get(this))
		{
			Subsystem->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDescriptionText);
		}
	}
}

void URsButton::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	
	if (URsUIManagerSubsystem* Subsystem = URsUIManagerSubsystem::Get(this))
	{
		Subsystem->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDescriptionText);
	}
}
