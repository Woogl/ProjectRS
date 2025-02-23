// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsAbilitySystemLibrary.generated.h"

class UGameplayAbility;
class URsGameplayAbility;
class UAbilitySystemComponent;
struct FGameplayTagContainer;
/**
 * 
 */
UCLASS()
class RS_API URsAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Find the first ability that matches tag. 
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library")
	static UGameplayAbility* FindAbilityWithTag(const UAbilitySystemComponent* AbilitySystemComponent, UPARAM(meta=(Categories="Ability"))FGameplayTagContainer AbilityTags, bool bExactMatch);

	// Find the first RS ability that matches tag. 
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library")
	static URsGameplayAbility* FindRsAbilityWithTag(const UAbilitySystemComponent* AbilitySystemComponent, UPARAM(meta=(Categories="Ability"))FGameplayTagContainer AbilityTags, bool bExactMatch);
	
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library")
	static void ModifyAbilityCooldownRemaining(const UAbilitySystemComponent* AbilitySystemComponent, UPARAM(meta=(Categories="Ability"))FGameplayTagContainer AbilityTags, bool bExactMatch, float TimeDiff);

	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library")
	static void SetAbilityCooldownRemaining(const UAbilitySystemComponent* AbilitySystemComponent, UPARAM(meta=(Categories="Ability"))FGameplayTagContainer AbilityTags, bool bExactMatch, float NewRemaining);
};
