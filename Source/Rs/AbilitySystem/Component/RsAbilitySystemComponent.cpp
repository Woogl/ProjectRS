// Copyright 2024 Team BH.


#include "RsAbilitySystemComponent.h"

#include "Rs/AbilitySystem/RsAbilitySet.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"

URsAbilitySystemComponent::URsAbilitySystemComponent()
{
	// Explicitly set the Ability System Component to replicate.
	SetIsReplicatedByDefault(true);
}

void URsAbilitySystemComponent::InitializeAbilitySystem(URsAbilitySet* AbilitySet, AActor* InOwnerActor, AActor* InAvatarActor)
{
	// Set the Owning Actor and Avatar Actor. (Used throughout the Gameplay Ability System to get references etc.)
	InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	
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

	// Grant abilities.
	for (const TSubclassOf<URsGameplayAbility> GameplayAbility : AbilitySet->GrantedAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameplayAbility, 0, INDEX_NONE, InOwnerActor);
		FGameplayAbilitySpecHandle GrantedAbilityHandle = GiveAbility(AbilitySpec);
		GrantedAbilityHandles.Add(GrantedAbilityHandle);
	}

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

void URsAbilitySystemComponent::SetupAbilityInputBindings()
{
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

