// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "RsAbilitySystemLibrary.generated.h"

class UGameplayAbility;
class URsGameplayAbility;
class UAbilitySystemComponent;
struct FGameplayTagContainer;

#define SET_SETBYCALLER_PROPERTY(SpecHandle, Property) \
UAbilitySystemBlueprintLibrary::AssignSetByCallerMagnitude(SpecHandle, GET_MEMBER_NAME_CHECKED(ThisClass, Property), Property)

#define GET_SETBYCALLER_PROPERTY(SpecHandle, Property, WarnIfNotFound, DefaultIfNotFound) \
((SpecHandle.IsValid() && SpecHandle.Data.IsValid()) ? \
SpecHandle.Data->GetSetByCallerMagnitude(GET_MEMBER_NAME_CHECKED(ThisClass, Property), WarnIfNotFound, DefaultIfNotFound) : DefaultIfNotFound)

/**
 * 
 */
UCLASS()
class RS_API URsAbilitySystemLibrary : public UAbilitySystemBlueprintLibrary
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
