// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "RsAbilitySystemLibrary.generated.h"

class URsAbilitySystemComponent;
class UGameplayAbility;
class URsGameplayAbility;
class UAbilitySystemComponent;
struct FGameplayTagContainer;

/**
 * 
 */
UCLASS()
class RS_API URsAbilitySystemLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()
	
public: /** Ability System Component */
	UFUNCTION(BlueprintPure, Category = "RS Ability System Library")
	static URsAbilitySystemComponent* GetRsAbilitySystemComponent(AActor* Actor);

	// Get ASC for applying environmental effects. All attribute is always 0.
	UFUNCTION(BlueprintPure, Category = "RS Ability System Library", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static UAbilitySystemComponent* GetWorldAbilitySystemComponent(const UObject* WorldContextObject);
	
	// Sends a replicated gameplay event to an actor.
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library", meta=(DisplayName="Send Gameplay Event To Actor (Replicated)"))
	static void SendGameplayEventToActor_Replicated(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload);

public:	/** Gameplay Ability */
	// Find the first ability that matches tags. 
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library", meta = (GameplayTagFilter = "Ability"))
	static UGameplayAbility* FindAbilityWithTags(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch);

	// Find the first RS ability that matches tags. 
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library", meta = (GameplayTagFilter = "Ability"))
	static URsGameplayAbility* FindRsAbilityWithTags(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch);
	
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library", meta = (GameplayTagFilter = "Ability"))
	static void ModifyAbilityCooldownRemaining(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch, float TimeDiff);

	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library", meta = (GameplayTagFilter = "Ability"))
	static void SetAbilityCooldownRemaining(const UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, bool bExactMatch, float NewRemaining);

public: /** Gameplay Effect */
	UFUNCTION(BlueprintCallable, Category = "RS Ability System Library", meta = (DataTablePin="DataTable"))
	static FActiveGameplayEffectHandle ApplyEffectByTable(const UDataTable* DataTable, FName RowName, UAbilitySystemComponent* Source, UAbilitySystemComponent* Target, FGameplayEffectContextHandle Context, float Level);
	
public: /** Gameplay Attribute */
	UFUNCTION(BlueprintPure, Category = "RS Ability System Library", meta = (GameplayTagFilter = "Stat,Coefficient"))
	static FGameplayAttribute GetAttributeByTag(FGameplayTag Tag);
	
	UFUNCTION(BlueprintPure, Category = "RS Ability System Library", meta = (GameplayTagFilter = "Stat"))
	static float GetNumericAttributeByTag(UAbilitySystemComponent* ASC, FGameplayTag StatTag);
};
