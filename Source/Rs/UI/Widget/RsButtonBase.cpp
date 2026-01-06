// Copyright 2025 Team BH.


#include "RsButtonBase.h"

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

FText URsButtonBase::GetButtonText() const
{
	if (Text_ButtonText)
	{
		return Text_ButtonText->GetText();
	}

	return FText::GetEmpty();
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
	
	SetButtonText(ButtonDisplayText);
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
	
	if (!ButtonDescriptionText.IsEmpty())
	{
		if (URsUIManagerSubsystem* Subsystem = URsUIManagerSubsystem::Get(this))
		{
			Subsystem->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDescriptionText);
		}
	}
}

void URsButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	
	if (URsUIManagerSubsystem* Subsystem = URsUIManagerSubsystem::Get(this))
	{
		Subsystem->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDescriptionText);
	}
}
