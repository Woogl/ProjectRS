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

void URsAbilitySystemComponent::InitializeAbilitySystem(URsAbilitySet* AbilitySet, AActor* InOwningActor, AActor* InAvatarActor)
{
	if (AbilitySystemDataInitialized)
	{
		return;
	}
	AbilitySystemDataInitialized = true;

	// Set the Owning Actor and Avatar Actor. (Used throughout the Gameplay Ability System to get references etc.)
	InitAbilityActorInfo(InOwningActor, InAvatarActor);

	// Apply the Gameplay Tag container as loose Gameplay Tags. (These are not replicated by default and should be applied on both server and client respectively.)
	AddLooseGameplayTags(AbilitySet->GrantedTags);
	
	// Check to see if we have authority. (Attribute Sets / Attribute Base Values / Gameplay Abilities / Gameplay Effects should only be added -or- set on authority and will be replicated to the client automatically.)
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
		GetOrCreateAttributeSubobject(AttributeSetClass);
	}

	// Set base attribute values.
	for (const TTuple<FGameplayAttribute, FScalableFloat>& AttributeBaseValue : AbilitySet->GrantedAttributes)
	{
		if (HasAttributeSetForAttribute(AttributeBaseValue.Key))
		{
			SetNumericAttributeBase(AttributeBaseValue.Key, AttributeBaseValue.Value.GetValueAtLevel(0.f));
		}
	}

	// Grant Gameplay Abilities.
	for (const TSubclassOf<URsGameplayAbility> GameplayAbility : AbilitySet->GrantedAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameplayAbility, 0, INDEX_NONE, this);
		GiveAbility(AbilitySpec);
	}

	// Apply Gameplay Effects.
	for (const TSubclassOf<UGameplayEffect>& GameplayEffect : AbilitySet->GrantedEffects)
	{
		if (!IsValid(GameplayEffect))
		{
			continue;
		}
			
		FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		if (FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingSpec(GameplayEffect, 1, EffectContextHandle); GameplayEffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), this);
		}
	}
}

