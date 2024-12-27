// Copyright 2024 Team BH.


#include "RsCharacterBase.h"

#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"

UAbilitySystemComponent* ARsCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

URsAbilitySystemComponent* ARsCharacterBase::GetRsAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
