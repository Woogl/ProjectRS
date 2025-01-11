// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsAbilitySystemLibrary.generated.h"

struct FGameplayEffectContext;
struct FGameplayEffectContextHandle;
struct FRsGameplayEffectContext;
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
	static UGameplayAbility* FindAbilityWithTag(const UAbilitySystemComponent* AbilitySystemComponent, UPARAM(meta=(Categories="Ability"))FGameplayTagContainer AbilityTags, bool bExactMatch = true);

	// Find the first RS ability that matches tag. 
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library")
	static URsGameplayAbility* FindRsAbilityWithTag(const UAbilitySystemComponent* AbilitySystemComponent, UPARAM(meta=(Categories="Ability"))FGameplayTagContainer AbilityTags, bool bExactMatch = true);

	UFUNCTION(BlueprintPure, Category = "RS Ability System Library")
	static FRsGameplayEffectContext ConvertToRsGameplayEffectContext(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool& OutIsCritical);
};
