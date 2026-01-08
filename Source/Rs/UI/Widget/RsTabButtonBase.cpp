// Copyright 2025 Team BH.


#include "RsTabButtonBase.h"

#include "CommonLazyImage.h"

void URsTabButtonBase::SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrushFromLazyDisplayAsset(LazyObject);
	}
}

void URsTabButtonBase::SetIconBrush(const FSlateBrush& Brush)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrush(Brush);
	}
}

void URsTabButtonBase::SetTabLabelInfo_Implementation(const FRsTabDescriptor& TabLabelInfo)
{
	SetButtonText(TabLabelInfo.TabText);
	SetIconBrush(TabLabelInfo.IconBrush);
}
