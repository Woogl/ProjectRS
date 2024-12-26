// Copyright 2024 Team BH.


#include "RsAbilitySystemComponent.h"

#include "Rs/AbilitySystem/Data/AbilitySystemData.h"
#include "Rs/Character/RsCharacterBase.h"

URsAbilitySystemComponent::URsAbilitySystemComponent()
{
	// Sets the Ability System Component to use "Mixed" replication mode. This will replicate minimal Gameplay Effects to Simulated Proxies and full info to everyone else.
	ReplicationMode = EGameplayEffectReplicationMode::Mixed;

	// Explicitly set the Ability System Component to replicate.
	SetIsReplicatedByDefault(true);
}

void URsAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	// Guard condition to ensure we should clear/init for this new Avatar Actor.
	const bool bAvatarHasChanged = AbilityActorInfo && AbilityActorInfo->AvatarActor != InAvatarActor && InAvatarActor != nullptr;
	
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bAvatarHasChanged)
	{
		if (ARsCharacterBase* RsCharacter = Cast<ARsCharacterBase>(InAvatarActor))
		{
			InitializeAbilitySystemData(RsCharacter->AbilitySystemInitializationData, InOwnerActor, InAvatarActor);
		}
	}
}

void URsAbilitySystemComponent::InitializeAbilitySystemData(const FAbilitySystemInitializationData& InitializationData, AActor* InOwningActor, AActor* InAvatarActor)
{
	if (AbilitySystemDataInitialized)
	{
		return;
	}
	AbilitySystemDataInitialized = true;

	// Set the Owning Actor and Avatar Actor. (Used throughout the Gameplay Ability System to get references etc.)
	InitAbilityActorInfo(InOwningActor, InAvatarActor);
	
	// Check to see if we have authority. (Attribute Sets / Attribute Base Values / Gameplay Abilities / Gameplay Effects should only be added -or- set on authority and will be replicated to the client automatically.)
	if (GetOwnerActor()->HasAuthority())
	{
		// Grant Attribute Sets if the array isn't empty.
		if (!InitializationData.AttributeSets.IsEmpty())
		{
			for (const TSubclassOf<UAttributeSet> AttributeSetClass : InitializationData.AttributeSets)
			{
				GetOrCreateAttributeSubobject(AttributeSetClass);
			}
		}

		// Set base attribute values if the map isn't empty.
		if (!InitializationData.AttributeBaseValues.IsEmpty())
		{
			for (const TTuple<FGameplayAttribute, float>& AttributeBaseValue : InitializationData.AttributeBaseValues)
			{
				if (HasAttributeSetForAttribute(AttributeBaseValue.Key))
				{
					SetNumericAttributeBase(AttributeBaseValue.Key, AttributeBaseValue.Value);
				}
			}
		}

		// Grant Gameplay Abilities if the array isn't empty.
		if (!InitializationData.GameplayAbilities.IsEmpty())
		{
			for (const TSubclassOf<UGameplayAbility> GameplayAbility : InitializationData.GameplayAbilities)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameplayAbility, 1, INDEX_NONE, this);
				
				GiveAbility(AbilitySpec);
			}
		}

		// Apply Gameplay Effects if the array isn't empty.
		if (!InitializationData.GameplayEffects.IsEmpty())
		{
			for (const TSubclassOf<UGameplayEffect>& GameplayEffect : InitializationData.GameplayEffects)
			{
				if (IsValid(GameplayEffect))
				{
					FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext();
					EffectContextHandle.AddSourceObject(this);

					if (FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingSpec(GameplayEffect, 1, EffectContextHandle); GameplayEffectSpecHandle.IsValid())
					{
						ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), this);
					}
				}
			}
		}
	}

	// Apply the Gameplay Tag container as loose Gameplay Tags. (These are not replicated by default and should be applied on both server and client respectively.)
	if (!InitializationData.GameplayTags.IsEmpty())
	{
		AddLooseGameplayTags(InitializationData.GameplayTags);
	}
}

