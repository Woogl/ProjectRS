// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RsAbilitySystemComponent.generated.h"

class URsAbilitySet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDealDamage, UAbilitySystemComponent*, TargetASC, FGameplayEffectSpecHandle, DamageEffectHandle);

/**
 * 
 */
UCLASS()
class RS_API URsAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	URsAbilitySystemComponent();

	void InitializeAbilitySet(URsAbilitySet* AbilitySet);
	
	void GrantAttribute(FGameplayAttribute Attribute, float BaseValue);
	void GrantAbility(TSubclassOf<UGameplayAbility> Ability);
	void GrantEffect(TSubclassOf<UGameplayEffect> Effect);
	void GrantTags(FGameplayTagContainer Tags);
	
	void SetupAbilityInputBindings();
	void TearDownAbilityInputBindings();

	static URsAbilitySystemComponent* GetAbilitySystemComponentFromActor(AActor* Actor);

	// Sends a replicated gameplay event to an actor.
	void SendGameplayEventToActor_Replicated(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload);

private:
	// Server RPC for sending gameplay events. 
	UFUNCTION(Server, Reliable)
	void SendGameplayEventToActor_Server(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload);

	// Multicast RPC for sending gameplay events. 
	UFUNCTION(NetMulticast, Reliable)
	void SendGameplayEventToActor_Multicast(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload);
	
	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GrantedEffectHandles;
};
