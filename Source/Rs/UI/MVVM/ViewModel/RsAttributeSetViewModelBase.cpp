// Copyright 2025 Team BH.


#include "RsAttributeSetViewModelBase.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

void URsAttributeSetViewModelBase::Initialize()
{
	Super::Initialize();

	if (const UAttributeSet* AttributeSet = GetModel<ThisClass>())
	{
		ASC = AttributeSet->GetOwningAbilitySystemComponent();
	}
}
