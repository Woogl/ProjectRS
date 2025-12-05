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
}

void URsAbilitySystemComponent::InitializeAbilitySet(URsAbilitySet* AbilitySet)
{
	if (!AbilitySet)
	{
		UE_LOG(RsLog, Warning, TEXT("%s has invalid ABS!"), *GetAvatarActor()->GetName());
		return;
	}
	
	if (!IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}
		
	GrantTags(AbilitySet->GrantedTags);
		
	for (auto [Attribute, BaseValue] : AbilitySet->GrantedAttributes)
	{
		GrantAttribute(Attribute, BaseValue.GetValueAtLevel(0));
	}
		
	for (const TSubclassOf<UGameplayAbility>& Ability : AbilitySet->GrantedAbilities)
	{
		GrantAbility(Ability);
	}

	for (const TSubclassOf<UGameplayEffect>& Effect : AbilitySet->GrantedEffects)
	{
		GrantEffect(Effect);
	}
		
	// Grant attributes from data table row.
	if (const FRsAttributeTableRow* Row = AbilitySet->GrantedAttributeTableRow.GetRow<FRsAttributeTableRow>(ANSI_TO_TCHAR(__FUNCTION__)))
	{
		for (const auto& [Tag, Attribute] : URsAbilitySystemSettings::Get().Attributes)
		{
			float BaseValue = Row->GetBaseValue(Attribute);
			GrantAttribute(Attribute, BaseValue);
		}
	}
}

void URsAbilitySystemComponent::NotifyAbilitySystemInitialized()
{
	bAbilitySystemInitialized = true;
	OnAbilitySystemInitialized.Broadcast();
}

void URsAbilitySystemComponent::GrantAttribute(FGameplayAttribute Attribute, float BaseValue)
{
	if (Attribute.IsValid())
	{
		if (UClass* AttributeSetClass = Attribute.GetAttributeSetClass())
		{
			if (!GetAttributeSubobject(AttributeSetClass))
			{
				UAttributeSet* NewSet = NewObject<UAttributeSet>(GetOwner(), AttributeSetClass);
				AddAttributeSetSubobject(NewSet);
			}
			SetNumericAttributeBase(Attribute, BaseValue);
		}
	}
}

void URsAbilitySystemComponent::GrantAbility(TSubclassOf<UGameplayAbility> Ability)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 0, INDEX_NONE, GetOwnerActor());
	FGameplayAbilitySpecHandle GrantedAbilityHandle = GiveAbility(AbilitySpec);
	GrantedAbilityHandles.Add(GrantedAbilityHandle);
}

void URsAbilitySystemComponent::GrantEffect(TSubclassOf<UGameplayEffect> Effect)
{
	FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	if (FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingSpec(Effect, 0, EffectContextHandle); GameplayEffectSpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle GrantedEffectHandle = ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), this);
		GrantedEffectHandles.Add(GrantedEffectHandle);
	}
}

void URsAbilitySystemComponent::GrantTags(FGameplayTagContainer Tags)
{
	// Apply the Gameplay Tag container as loose Gameplay Tags.
	// These are not replicated by default and should be applied on both server and client respectively.
	AddLooseGameplayTags(Tags);
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

void URsAbilitySystemComponent::SendGameplayEventToActor_Multicast_Implementation(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, Payload);
}

void URsAbilitySystemComponent::SendGameplayEventToActor_Server_Implementation(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload)
{
	SendGameplayEventToActor_Multicast(Actor, EventTag, Payload);
}

