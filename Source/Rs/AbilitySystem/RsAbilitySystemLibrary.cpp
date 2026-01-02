// Copyright 2024 Team BH.


#include "RsAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "RsAbilitySystemComponent.h"
#include "RsAbilitySystemGlobals.h"
#include "Abilities/RsGameplayAbility.h"
#include "Attributes/RsAttributeSetBase.h"
#include "Effect/RsEffectTable.h"
#include "Rs/RsLogChannels.h"

URsAbilitySystemComponent* URsAbilitySystemLibrary::GetRsAbilitySystemComponent(AActor* Actor)
{
	return URsAbilitySystemComponent::GetAbilitySystemComponentFromActor(Actor);
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

void URsAbilitySystemLibrary::SendGameplayEventToActor_Replicated(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload)
{
	if (Actor && EventTag.IsValid())
	{
		if (URsAbilitySystemComponent* RsASC = GetRsAbilitySystemComponent(Actor))
		{
			RsASC->SendGameplayEventToActor_Replicated(Actor, EventTag, Payload);
		}
	}
}

UGameplayAbility* URsAbilitySystemLibrary::FindAbilityWithTags(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch)
{
	if (AbilitySystemComponent == nullptr)
	{
		return nullptr;
	}

	TArray<FGameplayAbilitySpecHandle> OutHandles;
	AbilitySystemComponent->FindAllAbilitiesWithTags(OutHandles, AbilityTags, bExactMatch);
	if (OutHandles.IsEmpty())
	{
		UE_LOG(LogRsAbility, Warning, TEXT("Cannot find Ability: [%s]"), *AbilityTags.ToString());
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

URsGameplayAbility* URsAbilitySystemLibrary::FindRsAbilityWithTags(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch)
{
	if (UGameplayAbility* FoundAbility = FindAbilityWithTags(AbilitySystemComponent, AbilityTags, bExactMatch))
	{
		if (URsGameplayAbility* FoundRsAbility = Cast<URsGameplayAbility>(FoundAbility))
		{
			return FoundRsAbility;
		}
		else
		{
			UE_LOG(LogRsAbility, Warning, TEXT("Cannot find Ability: [%s]"), *AbilityTags.ToString());
		}
	}
	
	return nullptr;
}

void URsAbilitySystemLibrary::ModifyAbilityCooldownRemaining(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch, float TimeDiff)
{
	URsGameplayAbility* FoundRsAbility = FindRsAbilityWithTags(AbilitySystemComponent, AbilityTags, bExactMatch);
	if (FoundRsAbility)
	{
		FoundRsAbility->ModifyCooldownRemaining(TimeDiff);
	}
}

void URsAbilitySystemLibrary::SetAbilityCooldownRemaining(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch, float NewRemaining)
{
	URsGameplayAbility* FoundRsAbility = FindRsAbilityWithTags(AbilitySystemComponent, AbilityTags, bExactMatch);
	if (FoundRsAbility)
	{
		FoundRsAbility->SetCooldownRemaining(NewRemaining);
	}
}

FGameplayEffectSpecHandle URsAbilitySystemLibrary::MakeEffectSpecByTable(const UDataTable* DataTable, FName RowName, UAbilitySystemComponent* Source, float Level, FGameplayEffectContextHandle Context)
{
	if (const FRsEffectTableRowBase* Row = DataTable->FindRow<FRsEffectTableRowBase>(RowName, ANSI_TO_TCHAR(__FUNCTION__)))
	{
		FGameplayEffectSpecHandle Spec = Source->MakeOutgoingSpec(Row->EffectClass, Level, Context);
		FDataTableRowHandle TableRowHandle;
		TableRowHandle.DataTable = DataTable;
		TableRowHandle.RowName = RowName;
		URsAbilitySystemGlobals::SetEffectTableRowHandle(*Context.Get(), &TableRowHandle);
		return Spec;
	}
	return FGameplayEffectSpecHandle();
}

FActiveGameplayEffectHandle URsAbilitySystemLibrary::ApplyEffectByTable(const UDataTable* DataTable, FName RowName, UAbilitySystemComponent* Source, UAbilitySystemComponent* Target, float Level, FGameplayEffectContextHandle Context)
{
	if (const FRsEffectTableRowBase* Row = DataTable->FindRow<FRsEffectTableRowBase>(RowName, ANSI_TO_TCHAR(__FUNCTION__)))
	{
		FGameplayEffectContextHandle MyContext = Context.IsValid() ? Context : Source->MakeEffectContext();
		FDataTableRowHandle TableRowHandle;
		TableRowHandle.DataTable = DataTable;
		TableRowHandle.RowName = RowName;
		URsAbilitySystemGlobals::SetEffectTableRowHandle(*MyContext.Get(), &TableRowHandle);
		FGameplayEffectSpecHandle Spec = Source->MakeOutgoingSpec(Row->EffectClass, Level, MyContext);
		return Source->ApplyGameplayEffectSpecToTarget(*Spec.Data, Target);
	}
	return FActiveGameplayEffectHandle();
}

FGameplayAttribute URsAbilitySystemLibrary::GetAttributeByTag(FGameplayTag StatTag)
{
	return URsAttributeSetBase::TagToAttribute(StatTag);
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
