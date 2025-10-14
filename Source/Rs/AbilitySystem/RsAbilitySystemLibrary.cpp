// Copyright 2024 Team BH.


#include "RsAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"
#include "RsAbilitySystemComponent.h"
#include "RsAbilitySystemSettings.h"
#include "Abilities/RsGameplayAbility.h"
#include "Effect/RsEffectCoefficient.h"
#include "Rs/RsLogChannels.h"

URsAbilitySystemComponent* URsAbilitySystemLibrary::GetRsAbilitySystemComponent(AActor* OwningActor)
{
	return URsAbilitySystemComponent::GetAbilitySystemComponentFromActor(OwningActor);
}

UAbilitySystemComponent* URsAbilitySystemLibrary::GetWorldAbilitySystemComponent(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	const UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return nullptr;
	}
	const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(World->GetWorldSettings());
	if (ASI)
	{
		return ASI->GetAbilitySystemComponent();
	}
	return nullptr;
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

FGameplayEffectSpecHandle URsAbilitySystemLibrary::MakeEffectSpecCoefficient(UAbilitySystemComponent* SourceASC, const FRsEffectCoefficient& EffectCoefficient, FGameplayEffectContextHandle EffectContext)
{
	if (SourceASC && EffectCoefficient.IsValid())
	{
		FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(EffectCoefficient.EffectClass, 0, EffectContext);
		for (const TTuple<FGameplayTag, float>& Coeff : EffectCoefficient.Coefficients)
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(Coeff.Key, Coeff.Value);
		}
		return EffectSpecHandle;
	}
	return FGameplayEffectSpecHandle();
}

FActiveGameplayEffectHandle URsAbilitySystemLibrary::ApplyEffectCoefficient(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FRsEffectCoefficient& EffectCoefficient)
{
	if (SourceASC && EffectCoefficient.IsValid())
	{
		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecCoefficient = MakeEffectSpecCoefficient(SourceASC, EffectCoefficient, EffectContext);
		return ApplyEffectSpecCoefficient(SourceASC, TargetASC, EffectSpecCoefficient);
	}
	return FActiveGameplayEffectHandle();
}

FActiveGameplayEffectHandle URsAbilitySystemLibrary::ApplyEffectSpecCoefficient(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FGameplayEffectSpecHandle& EffectHandle)
{
	if (SourceASC && TargetASC)
	{
		if (FGameplayEffectSpec* EffectSpec = EffectHandle.Data.Get())
		{
			EffectSpec->GetContext().AddOrigin(TargetASC->GetAvatarActor()->GetActorLocation());
			if (SourceASC == TargetASC)
			{
				return SourceASC->ApplyGameplayEffectSpecToSelf(*EffectSpec);
			}
			else
			{
				return SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpec, TargetASC);
			}
		}
	}
	return FActiveGameplayEffectHandle();
}

FGameplayAttribute URsAbilitySystemLibrary::GetAttributeByTag(FGameplayTag StatTag)
{
	return URsAbilitySystemSettings::Get().Attributes.FindRef(StatTag);
}

float URsAbilitySystemLibrary::GetNumericAttributeByTag(UAbilitySystemComponent* ASC, FGameplayTag StatTag)
{
	if (!ASC)
	{
		return 0.f;
	}
	FGameplayAttribute Attribute = URsAbilitySystemSettings::Get().Attributes.FindRef(StatTag);
	return ASC->GetNumericAttribute(Attribute);
}

FGameplayAttribute URsAbilitySystemLibrary::GetAttributeByCoefficientTag(FGameplayTag CoefficientTag)
{
	for (const TTuple<FGameplayTag, FGameplayAttribute>& Coeff : URsAbilitySystemSettings::Get().Coefficients)
	{
		if (CoefficientTag.MatchesTag(Coeff.Key))
		{
			return Coeff.Value;
		}
	}
	return FGameplayAttribute();
}
