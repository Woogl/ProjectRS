// Copyright 2024 Team BH.


#include "RsAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "RsAbilitySystemComponent.h"
#include "Abilities/RsGameplayAbility.h"
#include "Effect/RsEffectCoefficient.h"
#include "Rs/RsLogChannels.h"

URsAbilitySystemComponent* URsAbilitySystemLibrary::GetRsAbilitySystemComponent(AActor* OwningActor)
{
	return URsAbilitySystemComponent::GetAbilitySystemComponentFromActor(OwningActor);
}

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

FGameplayEffectSpecHandle URsAbilitySystemLibrary::MakeEffectSpecCoefficient(UAbilitySystemComponent* SourceASC, const FRsEffectCoefficient& EffectCoefficient, FGameplayEffectContextHandle InEffectContext)
{
	if (SourceASC && EffectCoefficient.IsValid())
	{
		FGameplayEffectContextHandle EffectContext = InEffectContext.IsValid() ? InEffectContext : SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(EffectCoefficient.EffectClass, 0, EffectContext);
		for (const TTuple<FGameplayTag, float>& Coefficient : EffectCoefficient.Coefficients)
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(Coefficient.Key, Coefficient.Value);
		}
		return EffectSpecHandle;
	}
	return FGameplayEffectSpecHandle();
}

FActiveGameplayEffectHandle URsAbilitySystemLibrary::ApplyEffectCoefficient(const AActor* Source, const AActor* Target, const FRsEffectCoefficient& EffectCoefficient)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Source);
	
	if (SourceASC && EffectCoefficient.IsValid())
	{
		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(EffectCoefficient.EffectClass, 0, EffectContext);
		for (const TTuple<FGameplayTag, float>& Coefficient : EffectCoefficient.Coefficients)
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(Coefficient.Key, Coefficient.Value);
		}
		return ApplyEffectSpecCoefficient(Source, Target, EffectSpecHandle);
	}
	return FActiveGameplayEffectHandle();
}

FActiveGameplayEffectHandle URsAbilitySystemLibrary::ApplyEffectSpecCoefficient(const AActor* Source, const AActor* Target, const FGameplayEffectSpecHandle& EffectHandle)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Source);
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	
	if (SourceASC && TargetASC)
	{
		if (FGameplayEffectSpec* EffectSpec = EffectHandle.Data.Get())
		{
			EffectSpec->GetContext().AddOrigin(Target->GetActorLocation());
			return SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpec, TargetASC);
		}
	}
	return FActiveGameplayEffectHandle();
}
