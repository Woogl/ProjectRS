// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "RsAbilitySystemLibrary.generated.h"

struct FRsEffectCoefficient;
class URsAbilitySystemComponent;
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
	
public: /** Ability System Component */
	
	UFUNCTION(BlueprintPure, Category = "RS Ability System Library")
	static URsAbilitySystemComponent* GetRsAbilitySystemComponent(AActor* OwningActor);

	// Get ASC for applying environmental effects. All attribute is always 0.
	UFUNCTION(BlueprintPure, Category = "RS Ability System Library", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static UAbilitySystemComponent* GetWorldAbilitySystemComponent(const UObject* WorldContextObject);

public:	/** Gameplay Ability */
	
	// Find the first ability that matches tags. 
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library", meta = (GameplayTagFilter = "Ability"))
	static UGameplayAbility* FindAbilityWithTag(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch);

	// Find the first RS ability that matches tags. 
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library", meta = (GameplayTagFilter = "Ability"))
	static URsGameplayAbility* FindRsAbilityWithTag(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch);
	
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library", meta = (GameplayTagFilter = "Ability"))
	static void ModifyAbilityCooldownRemaining(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch, float TimeDiff);

	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library", meta = (GameplayTagFilter = "Ability"))
	static void SetAbilityCooldownRemaining(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch, float NewRemaining);

public: /** Gameplay Effect */
	
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library")
	static FGameplayEffectSpecHandle MakeEffectSpecCoefficient(UAbilitySystemComponent* SourceASC, const FRsEffectCoefficient& EffectCoefficient, FGameplayEffectContextHandle EffectContext);
	
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library")
	static FActiveGameplayEffectHandle ApplyEffectCoefficient(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FRsEffectCoefficient& EffectCoefficient);

	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library")
	static FActiveGameplayEffectHandle ApplyEffectSpecCoefficient(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FGameplayEffectSpecHandle& EffectHandle);

public: /** Gameplay Attribute */

	UFUNCTION(BlueprintPure, Category = "RS Ability System Library", meta = (GameplayTagFilter = "Stat"))
	static FGameplayAttribute GetAttributeByTag(FGameplayTag StatTag);
	
	UFUNCTION(BlueprintPure, Category = "RS Ability System Library", meta = (GameplayTagFilter = "Stat"))
	static float GetNumericAttributeByTag(UAbilitySystemComponent* ASC, FGameplayTag StatTag);

	// ex. Coefficient.ATK.Source -> AttackAttribute
	UFUNCTION(BlueprintPure, Category = "RS Ability System Library", meta = (GameplayTagFilter = "Coefficient"))
	static FGameplayAttribute GetAttributeByCoefficientTag(FGameplayTag CoefficientTag);
};
