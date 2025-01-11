// Copyright 2024 Team BH.


#include "RsAbilitySystemGlobals.h"

#include "Effect/RsGameplayEffectContext.h"

FGameplayEffectContext* URsAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRsGameplayEffectContext();
}
