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
	
	FOnDealDamage OnDealDamage;

	void InitializeAbilitySystem(URsAbilitySet* AbilitySet, AActor* InOwnerActor, AActor* InAvatarActor);
	
	void SetupAbilityInputBindings();
	void TearDownAbilityInputBindings();

private:
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
