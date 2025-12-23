// Copyright 2024 Team BH.


#include "RsAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "RsAbilitySystemGlobals.h"
#include "RsAbilitySystemSettings.h"
#include "Attributes/RsAttributeTableRow.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySet.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"

URsAbilitySystemComponent::URsAbilitySystemComponent()
{
	// Explicitly set the Ability System Component to replicate.
	SetIsReplicatedByDefault(true);
	
	bReplicateUsingRegisteredSubObjectList = true;
}

void URsAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	
	if (!IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}
	
	if (AbilitySets.IsEmpty())
	{
		UE_LOG(LogRsAbility, Error, TEXT("[%s]'s ABS is empty!"), *InOwnerActor->GetActorLabel());
		return;
	}

	if (bAbilitySystemInitialized == true)
	{
		return;
	}
	
	for (URsAbilitySet* AbilitySet : AbilitySets)
	{
		InitAbilitySet(AbilitySet);
	}
	bAbilitySystemInitialized = true;
	OnAbilitySystemInitialized.Broadcast();
}

void URsAbilitySystemComponent::SetupAbilityInputBindings()
{
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpecHandle& AbilityHandle : GrantedAbilityHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilityHandle))
		{
			if (URsGameplayAbility* RsAbility = Cast<URsGameplayAbility>(AbilitySpec->Ability))
			{
				RsAbility->SetupEnhancedInputBindings(AbilityActorInfo.Get(), *AbilitySpec);
			}
		}
	}
}

void URsAbilitySystemComponent::TearDownAbilityInputBindings()
{
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpecHandle& AbilityHandle : GrantedAbilityHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilityHandle))
		{
			if (URsGameplayAbility* RsAbility = Cast<URsGameplayAbility>(AbilitySpec->Ability))
			{
				RsAbility->TeardownEnhancedInputBindings(AbilityActorInfo.Get(), *AbilitySpec);
			}
		}
	}
}

URsAbilitySystemComponent* URsAbilitySystemComponent::GetAbilitySystemComponentFromActor(AActor* Actor)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor))
	{
		return Cast<URsAbilitySystemComponent>(ASC);
	}
	return nullptr;
}

void URsAbilitySystemComponent::CallOrRegister_OnAbilitySystemInitialized(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (bAbilitySystemInitialized)
	{
		Delegate.Execute();
	}
	else if (!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemInitialized.Add(Delegate);
	}
}

void URsAbilitySystemComponent::SendGameplayEventToActor_Replicated(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload)
{
	if (IsValid(Actor) && EventTag.IsValid())
	{
		if (Actor->HasAuthority())
		{
			SendGameplayEventToActor_Multicast(Actor, EventTag, Payload);
		}
		else
		{
			SendGameplayEventToActor_Server(Actor, EventTag, Payload);
		}
	}
}

void URsAbilitySystemComponent::InitAbilitySet(URsAbilitySet* AbilitySet)
{
	if (!AbilitySet)
	{
		UE_LOG(LogRs, Warning, TEXT("%s has invalid ABS!"), *GetAvatarActor()->GetName());
		return;
	}

	if (AbilitySet->GrantedAttributeTableRow.IsNull())
	{
		for (auto [Attribute, BaseValue] : AbilitySet->GrantedAttributes)
		{
			InitAttribute(Attribute, BaseValue.GetValueAtLevel(0));
		}
	}
	else
	{
		if (const FRsAttributeTableRow* Row = AbilitySet->GrantedAttributeTableRow.GetRow<FRsAttributeTableRow>(ANSI_TO_TCHAR(__FUNCTION__)))
		{
			for (const auto [Tag, Attribute] : URsAttributeSetBase::GetStatMap())
			{
				float BaseValue = Row->GetBaseValue(Attribute);
				InitAttribute(Attribute, BaseValue);
			}
		}
	}
		
	for (const TSubclassOf<UGameplayAbility>& Ability : AbilitySet->GrantedAbilities)
	{
		InitAbility(Ability);
	}

	for (const TSubclassOf<UGameplayEffect>& Effect : AbilitySet->GrantedEffects)
	{
		InitEffect(Effect);
	}
	
	InitTags(AbilitySet->GrantedTags);
}

void URsAbilitySystemComponent::InitAttribute(FGameplayAttribute Attribute, float BaseValue)
{
	if (!Attribute.IsValid())
	{
		UE_LOG(LogRsAbility, Warning, TEXT("[%s] has invalid attribute in ABS!"), *GetAvatarActor()->GetName());
		return;
	}

	if (!GetAttributeSet(Attribute.GetAttributeSetClass()))
	{
		UAttributeSet* NewSet = NewObject<UAttributeSet>(GetOwner(), Attribute.GetAttributeSetClass());
		AddAttributeSetSubobject(NewSet);
	}
	SetNumericAttributeBase(Attribute, BaseValue);
}

void URsAbilitySystemComponent::InitAbility(TSubclassOf<UGameplayAbility> Ability)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 0, INDEX_NONE, GetOwnerActor());
	FGameplayAbilitySpecHandle Handle = GiveAbility(AbilitySpec);
	GrantedAbilityHandles.Add(Handle);
}

void URsAbilitySystemComponent::InitEffect(TSubclassOf<UGameplayEffect> Effect)
{
	FActiveGameplayEffectHandle Handle = BP_ApplyGameplayEffectToSelf(Effect, 0, MakeEffectContext());
	if (Handle.IsValid())
	{
		GrantedEffectHandles.Add(Handle);
	}
}

void URsAbilitySystemComponent::InitTags(FGameplayTagContainer Tags)
{
	// Apply the Gameplay Tag container as loose Gameplay Tags.
	// These are not replicated by default and should be applied on both server and client respectively.
	AddLooseGameplayTags(Tags);
}

void URsAbilitySystemComponent::SendGameplayEventToActor_Multicast_Implementation(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, Payload);
}

void URsAbilitySystemComponent::SendGameplayEventToActor_Server_Implementation(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload)
{
	SendGameplayEventToActor_Multicast(Actor, EventTag, Payload);
}

