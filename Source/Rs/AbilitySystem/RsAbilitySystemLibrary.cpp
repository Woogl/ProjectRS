// Copyright 2024 Team BH.


#include "RsAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "RsAbilitySystemComponent.h"
#include "RsAbilitySystemSettings.h"
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

FGameplayEffectSpecHandle URsAbilitySystemLibrary::MakeEffectSpecCoefficient(UAbilitySystemComponent* SourceASC, const FRsEffectCoefficient& EffectCoefficient, FGameplayEffectContextHandle EffectContext)
{
	if (SourceASC && EffectCoefficient.IsValid())
	{
		FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(EffectCoefficient.EffectClass, 0, EffectContext);
		for (const FRsStatCoefficient& Coeff : EffectCoefficient.Coefficients)
		{
			FString Suffix = StaticEnum<EGameplayEffectAttributeCaptureSource>()->GetNameStringByValue((int64)Coeff.CaptureSource);
			for (const TTuple<FGameplayTag, float>& StatCoefficient : Coeff.StatCoefficients)
			{
				// ex: Stat.ATK.Source
				FName SetByCallerName = FName(StatCoefficient.Key.ToString() + TEXT(".")+ Suffix);
				EffectSpecHandle.Data->SetSetByCallerMagnitude(SetByCallerName, StatCoefficient.Value);
			}
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
			return SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpec, TargetASC);
		}
	}
	return FActiveGameplayEffectHandle();
}

float URsAbilitySystemLibrary::GetNumericAttributeByTag(UAbilitySystemComponent* ASC, FGameplayTag StatTag)
{
	if (!ASC)
	{
		return 0.f;
	}
	FGameplayAttribute Attribute = URsAbilitySystemSettings::Get().TaggedAttributes.FindRef(StatTag);
	return ASC->GetNumericAttribute(Attribute);
}
