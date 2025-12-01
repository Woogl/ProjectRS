// Copyright 2024 Team BH.


#include "RsAbilitySystemComponent.h"

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

void URsAbilitySystemComponent::InitializeAbilitySystem(TArray<URsAbilitySet*> AbilitySets, AActor* InOwnerActor, AActor* InAvatarActor)
{
	// Set the Owning Actor and Avatar Actor. (Used throughout the Gameplay Ability System to get references etc.)
	InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (AbilitySets.IsEmpty())
	{
		UE_LOG(RsLog, Error, TEXT("%s does not have ABS!"), *InAvatarActor->GetName());
		return;
	}
	
	// Attribute Sets / Attribute Base Values / Gameplay Abilities / Gameplay Effects should only be added -or- set on authority and will be replicated to the client automatically.
	if (!GetOwnerActor()->HasAuthority())
	{
		return;
	}

	for (URsAbilitySet* AbilitySet : AbilitySets)
	{
		if (!AbilitySet)
		{
			UE_LOG(RsLog, Warning, TEXT("%s has invalid ABS!"), *InAvatarActor->GetName());
			continue;
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
}

void URsAbilitySystemComponent::GrantAttribute(FGameplayAttribute Attribute, float BaseValue)
{
	if (Attribute.IsValid())
	{
		if (UClass* AttributeSetClass = Attribute.GetAttributeSetClass())
		{
			const UAttributeSet* GrantedAttributeSet = GetOrCreateAttributeSubobject(AttributeSetClass);
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

URsAbilitySystemComponent* URsAbilitySystemComponent::GetAbilitySystemComponentFromActor(AActor* OwningActor)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
	{
		return Cast<URsAbilitySystemComponent>(ASC);
	}
	return nullptr;
}

