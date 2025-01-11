// Copyright 2024 Team BH.


#include "RsAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Abilities/RsGameplayAbility.h"
#include "Effect/RsGameplayEffectContext.h"

UGameplayAbility* URsAbilitySystemLibrary::FindAbilityWithTag(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch)
{
	if (AbilitySystemComponent == nullptr)
	{
		return nullptr;
	}

	TArray<FGameplayAbilitySpecHandle> OutHandles;
	AbilitySystemComponent->FindAllAbilitiesWithTags(OutHandles, AbilityTags, bExactMatch);
	if (OutHandles.IsEmpty())
	{
		return nullptr;
	}

	FGameplayAbilitySpec* FoundSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(OutHandles[0]);
	UGameplayAbility* AbilityInstance = FoundSpec->GetPrimaryInstance();
	if (AbilityInstance == nullptr)
	{
		// default to the CDO if we can't
		return FoundSpec->Ability;
	}
	return AbilityInstance;
}

URsGameplayAbility* URsAbilitySystemLibrary::FindRsAbilityWithTag(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch)
{
	UGameplayAbility* FoundAbility = FindAbilityWithTag(AbilitySystemComponent, AbilityTags, bExactMatch);
	if (FoundAbility)
	{
		return Cast<URsGameplayAbility>(FoundAbility);
	}
	return nullptr;
}

FRsGameplayEffectContext URsAbilitySystemLibrary::ConvertToRsGameplayEffectContext(FGameplayEffectContextHandle& EffectContextHandle, bool& OutIsCritical)
{
	if (FGameplayEffectContext* EffectContext = EffectContextHandle.Get())
	{
		if (EffectContext->GetScriptStruct() == FRsGameplayEffectContext::StaticStruct())
		{
			FRsGameplayEffectContext RsEffectContext = *static_cast<FRsGameplayEffectContext*>(EffectContextHandle.Get());
			OutIsCritical = RsEffectContext.bIsCriticalHit;
			return *static_cast<FRsGameplayEffectContext*>(EffectContextHandle.Get());
		}
	}
	return FRsGameplayEffectContext();
}
