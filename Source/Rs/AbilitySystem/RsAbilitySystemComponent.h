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

	void InitializeAbilitySystem(TArray<URsAbilitySet*> AbilitySets, AActor* InOwnerActor, AActor* InAvatarActor);
	
	void GrantAttribute(FGameplayAttribute Attribute, float BaseValue);
	void GrantAbility(TSubclassOf<UGameplayAbility> Ability);
	void GrantEffect(TSubclassOf<UGameplayEffect> Effect);
	void GrantTags(FGameplayTagContainer Tags);
	
	void SetupAbilityInputBindings();
	void TearDownAbilityInputBindings();

	static URsAbilitySystemComponent* GetAbilitySystemComponentFromActor(AActor* OwningActor);

private:
	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GrantedEffectHandles;
};
