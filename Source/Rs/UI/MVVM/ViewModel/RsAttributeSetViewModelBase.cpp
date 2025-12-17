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
		if (ASC.IsValid())
		{
			TArray<FGameplayAttribute> OutAttributes;
			AttributeSet->GetAttributesFromSetClass(AttributeSet->GetClass(), OutAttributes);
			for (const FGameplayAttribute& Attribute : OutAttributes)
			{
				// Data binding
				ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &ThisClass::HandleAttributeChanged);
			}
		}
	}
}

void URsAttributeSetViewModelBase::Deinitialize()
{
	if (const UAttributeSet* AttributeSet = GetModel<ThisClass>())
	{
		if (ASC.IsValid())
		{
			TArray<FGameplayAttribute> OutAttributes;
			AttributeSet->GetAttributesFromSetClass(AttributeSet->GetClass(), OutAttributes);
			for (const FGameplayAttribute& Attribute : OutAttributes)
			{
				// Data unbinding
				ASC->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
			}
		}
	}
	
	Super::Deinitialize();
}

void URsAttributeSetViewModelBase::HandleAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeChanged.Broadcast(Data.Attribute, Data.OldValue, Data.NewValue);
}
