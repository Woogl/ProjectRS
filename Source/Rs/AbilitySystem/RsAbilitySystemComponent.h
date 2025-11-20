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

	void InitializeAbilitySystem(URsAbilitySet* AbilitySet, AActor* InOwnerActor, AActor* InAvatarActor);
	
	void SetupAbilityInputBindings();
	void TearDownAbilityInputBindings();

	static URsAbilitySystemComponent* GetAbilitySystemComponentFromActor(AActor* OwningActor);

	FGameplayEffectSpecHandle MakeOutgoingSpecFromSharedTable(FName RowName, float EffectLevel);

private:
	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GrantedEffectHandles;
};
