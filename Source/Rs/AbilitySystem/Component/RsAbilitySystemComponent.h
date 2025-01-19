// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RsAbilitySystemComponent.generated.h"

class URsAbilitySet;

/**
 * 
 */
UCLASS()
class RS_API URsAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	URsAbilitySystemComponent();

	// Called to initialize an Ability System Component with the supplied data.
	// Call this on the Server and Client to properly init references / values.
	void InitializeAbilitySystem(URsAbilitySet* AbilitySet, AActor* InOwnerActor, AActor* InAvatarActor);
	
	void UninitializeAbilitySystem();

	FGameplayEventMulticastDelegate OnAnyGameplayEvent;
	
private:
	bool AbilitySystemDataInitialized = false;

	virtual int32 HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) override;

	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GrantedEffectHandles;

	// Pointers to the granted attribute sets.
	UPROPERTY()
	TArray<const UAttributeSet*> GrantedAttributeSets;
};
