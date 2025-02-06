// Copyright 2024 Team BH.


#include "RsAbilitySystemComponent.h"

#include "Rs/AbilitySystem/Data/RsAbilitySet.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"

URsAbilitySystemComponent::URsAbilitySystemComponent()
{
	// Explicitly set the Ability System Component to replicate.
	SetIsReplicatedByDefault(true);
}

void URsAbilitySystemComponent::GrantTags(URsAbilitySet* AbilitySet, AActor* InOwnerActor, AActor* InAvatarActor)
{
	// Apply the Gameplay Tag container as loose Gameplay Tags. (These are not replicated by default and should be applied on both server and client respectively.)
	AddLooseGameplayTags(AbilitySet->GrantedTags);
}

void URsAbilitySystemComponent::GrantAttributes(URsAbilitySet* AbilitySet, AActor* InOwnerActor, AActor* InAvatarActor)
{
	if (!GetOwnerActor()->HasAuthority())
	{
		return;
	}
	
	// Grant attribute sets.
	TSet<UClass*> AttributeSetClasses;
	for	(TTuple<FGameplayAttribute, FScalableFloat> GrantedAttribute : AbilitySet->GrantedAttributes)
	{
		if (UClass* AttributeSetClass = GrantedAttribute.Key.GetAttributeSetClass())
		{
			AttributeSetClasses.Add(AttributeSetClass);
		}
	}
	for (UClass* AttributeSetClass : AttributeSetClasses)
	{
		const UAttributeSet* GrantedAttributeSet = GetOrCreateAttributeSubobject(AttributeSetClass);
		GrantedAttributeSets.Add(GrantedAttributeSet);
	}
		
	// Set base attribute values.
	for (const TTuple<FGameplayAttribute, FScalableFloat>& AttributeBaseValue : AbilitySet->GrantedAttributes)
	{
		if (HasAttributeSetForAttribute(AttributeBaseValue.Key))
		{
			SetNumericAttributeBase(AttributeBaseValue.Key, AttributeBaseValue.Value.GetValueAtLevel(0.f));
		}
	}
}

void URsAbilitySystemComponent::GrantAbilities(URsAbilitySet* AbilitySet, AActor* InOwnerActor, AActor* InAvatarActor)
{
	if (!GetOwnerActor()->HasAuthority())
	{
		return;
	}
	
	for (const TSubclassOf<URsGameplayAbility> GameplayAbility : AbilitySet->GrantedAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameplayAbility, 0, INDEX_NONE, InOwnerActor);
		FGameplayAbilitySpecHandle GrantedAbilityHandle = GiveAbility(AbilitySpec);
		GrantedAbilityHandles.Add(GrantedAbilityHandle);
	}
}

void URsAbilitySystemComponent::GrantEffects(URsAbilitySet* AbilitySet, AActor* InOwnerActor, AActor* InAvatarActor)
{
	// Apply Gameplay Effects.
	for (const TSubclassOf<UGameplayEffect>& GameplayEffect : AbilitySet->GrantedEffects)
	{
		FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		if (FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingSpec(GameplayEffect, 0, EffectContextHandle); GameplayEffectSpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GrantedEffectHandle = ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), this);
			GrantedEffectHandles.Add(GrantedEffectHandle);
		}
	}
}

int32 URsAbilitySystemComponent::HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	FGameplayEventData OutPayload;
	OutPayload.EventTag = EventTag;
	OnAnyGameplayEvent.Broadcast(&OutPayload);
	
	return Super::HandleGameplayEvent(EventTag, Payload);
}

