// Copyright 2024 Team BH.


#include "RsAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Abilities/RsGameplayAbility.h"
#include "Rs/RsLogChannels.h"

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
		UE_LOG(RsLog, Warning, TEXT("Cannot find Ability: %s"), *AbilityTags.ToString());
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
	if (UGameplayAbility* FoundAbility = FindAbilityWithTag(AbilitySystemComponent, AbilityTags, bExactMatch))
	{
		if (URsGameplayAbility* FoundRsAbility = Cast<URsGameplayAbility>(FoundAbility))
		{
			return FoundRsAbility;
		}
		else
		{
			UE_LOG(RsLog, Warning, TEXT("Cannot find Ability: %s"), *AbilityTags.ToString());
		}
	}
	
	return nullptr;
}

void URsAbilitySystemLibrary::ModifyAbilityCooldownRemaining(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch, float TimeDiff)
{
	URsGameplayAbility* FoundRsAbility = FindRsAbilityWithTag(AbilitySystemComponent, AbilityTags, bExactMatch);
	if (FoundRsAbility)
	{
		FoundRsAbility->ModifyCooldownRemaining(TimeDiff);
	}
}

void URsAbilitySystemLibrary::SetAbilityCooldownRemaining(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch, float NewRemaining)
{
	URsGameplayAbility* FoundRsAbility = FindRsAbilityWithTag(AbilitySystemComponent, AbilityTags, bExactMatch);
	if (FoundRsAbility)
	{
		FoundRsAbility->SetCooldownRemaining(NewRemaining);
	}
}
