// Copyright 2024 Team BH.


#include "RsAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "RsAbilitySystemComponent.h"
#include "RsAbilitySystemGlobals.h"
#include "RsAbilitySystemSettings.h"
#include "Abilities/RsGameplayAbility.h"
#include "Effect/RsEffectTable.h"
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

FActiveGameplayEffectHandle URsAbilitySystemLibrary::ApplyEffectByTable(UDataTable* DataTable, FName RowName, UAbilitySystemComponent* Source, UAbilitySystemComponent* Target, float Level)
{
	if (FRsEffectTableRowBase* Row = DataTable->FindRow<FRsEffectTableRow>(RowName, ANSI_TO_TCHAR(__FUNCTION__)))
	{
		FGameplayEffectSpecHandle Spec = Source->MakeOutgoingSpec(Row->EffectClass, Level, Source->MakeEffectContext());
		FDataTableRowHandle TableRowHandle;
		TableRowHandle.DataTable = DataTable;
		TableRowHandle.RowName = RowName;
		URsAbilitySystemGlobals::SetSetByCallerTableRowHandle(*Spec.Data, &TableRowHandle);
		return Source->ApplyGameplayEffectSpecToTarget(*Spec.Data, Target);
	}
	return FActiveGameplayEffectHandle();
}

FGameplayAttribute URsAbilitySystemLibrary::GetAttributeByTag(FGameplayTag StatTag)
{
	if (const FGameplayAttribute* FoundStat = URsAbilitySystemSettings::Get().Attributes.Find(StatTag))
	{
		return *FoundStat;
	}
	return nullptr;
}

float URsAbilitySystemLibrary::GetNumericAttributeByTag(UAbilitySystemComponent* ASC, FGameplayTag StatTag)
{
	if (!ASC)
	{
		return 0.f;
	}
	
	FGameplayAttribute Attribute = GetAttributeByTag(StatTag);
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
