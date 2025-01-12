// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Abilities/GameplayAbility.h"
#include "RsGameplayAbility.generated.h"

class ARsCharacterBase;
/**
 * 
 */
UCLASS()
class RS_API URsGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	URsGameplayAbility();
	
	// Tells an ability to activate immediately when it's granted. (Useful for passive abilities and abilities forced on others)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RS")
	bool ActivateAbilityOnGranted = false;

	// Which Input Action to bind the activation event to.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RS")
	TObjectPtr<UInputAction> ActivationInputAction = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldowns", meta = (Categories = "Cooldown"))
	FGameplayTag CooldownTag;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldowns")
	FScalableFloat CooldownDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Costs")
	FScalableFloat Cost;

	// Returns the "Avatar Character" associated with this Gameplay Ability.
	// Will return null if the Avatar Actor does not derive from Character.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ARsCharacterBase* GetAvatarCharacter() const { return AvatarCharacter.Get(); }

	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

protected:
	// Keep a pointer to "Avatar Character" so we don't have to cast to Character in instanced abilities owned by a Character derived class.
	TWeakObjectPtr<ARsCharacterBase> AvatarCharacter = nullptr;
	
	// Think of this as "BeginPlay".
	// Add logic here that should run when the Ability is first initialized.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Called to bind Input Pressed and Input Released events to the Avatar Actor's Enhanced Input Component if it is reachable. 
	void SetupEnhancedInputBindings(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	// Called when the "Activation Input Action" is triggered.
	void HandleInputPressedEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle SpecHandle);

	// Called when the "Activation Input Action" is completed.
	void HandleInputReleasedEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle SpecHandle);

	// Override "OnRemoveAbility" to clean up Enhanced Input Bindings.
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void HandleGameplayEvent(const FGameplayEventData* EventData);

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnGiveAbility")
	void K2_OnGiveAbility();

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnRemoveAbility")
	void K2_OnRemoveAbility();

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnGameplayEvent")
	void K2_OnGameplayEvent(FGameplayTag EventTag);

private:
	UPROPERTY(Transient)
	mutable FGameplayTagContainer CachedCooldownTags;
};
