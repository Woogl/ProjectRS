// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "RsAbilityEventInfo.h"
#include "Abilities/GameplayAbility.h"
#include "RsGameplayAbility.generated.h"

class URsGenericContainer;
class ARsCharacterBase;

USTRUCT(BlueprintType)
struct FRsGameplayMessage
{
	GENERATED_BODY()
	
};

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
	UPROPERTY(EditDefaultsOnly, Category = "RS")
	bool bActivateOnGranted = false;

	// Which Input Action to bind the activation event to.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RS")
	TObjectPtr<UInputAction> ActivationInputAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "RS", meta=(DisplayThumbnail="true", AllowedClasses="MaterialInterface,Texture2D"))
	TObjectPtr<UObject> SkillIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cooldowns", meta = (Categories = "Cooldown"))
	FGameplayTag CooldownTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cooldowns")
	float CooldownDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cooldowns", meta = (ClampMin = "0"))
	int32 MaxRechargeStacks = 0;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRechargeStacksChanged, int32 NewStacks);
	FOnRechargeStacksChanged OnRechargeStacksChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Costs")
	float CostAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Costs")
	TSubclassOf<UGameplayEffect> CostRecoveryEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Costs")
	float CostRecoveryAmount;

	// Returns the "Avatar Character" associated with this Gameplay Ability.
	// Will return null if the Avatar Actor does not derive from Character.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ARsCharacterBase* GetAvatarCharacter() const { return AvatarCharacter.Get(); }

	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	void ApplyCostRecovery();
	
	UFUNCTION(BlueprintCallable, Category = "Cooldowns")
	void ModifyCooldownRemaining(float TimeDiff);

	UFUNCTION(BlueprintCallable, Category = "Cooldowns")
	void SetCooldownRemaining(float NewRemaining);

	UFUNCTION(BlueprintPure, Category = "Cooldowns")
	int32 GetCurrentRechargeStacks() const { return CurrentRechargeStacks; }
	
	UFUNCTION(BlueprintCallable, Category = "Cooldowns")
	void ModifyCurrentRechargeStacks(int32 Diff);

	UFUNCTION(BlueprintCallable, Category = "Cooldowns")
	void SetCurrentRechargeStacks(int32 NewStacks);
	
	// Called to bind Input Pressed and Input Released events to the Avatar Actor's Enhanced Input Component if it is reachable. 
	void SetupEnhancedInputBindings(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	// Clear the bindings from the Enhanced Input Component.
	void TeardownEnhancedInputBindings(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	// Contains state values. Useful for storing data between anim notifies.
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URsGenericContainer> StatesContainer;

	const TArray<FRsAbilityEventInfo>& GetAbilityEvents() const { return AbilityEvents; }
	
protected:
	// Keep a pointer to "Avatar Character" so we don't have to cast to Character in instanced abilities owned by a Character derived class.
	TWeakObjectPtr<ARsCharacterBase> AvatarCharacter = nullptr;
	
	// Think of this as "BeginPlay".
	// Add logic here that should run when the Ability is first initialized.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	// Called when the "Activation Input Action" is triggered.
	void HandleInputPressedEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle SpecHandle);

	// Called when the "Activation Input Action" is completed.
	void HandleInputReleasedEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle SpecHandle);

	// Called when cooldown stacks changed.
	void HandleRechargeStacksChanged(FGameplayTag GameplayTag, int NewStacks);

	// Override "OnRemoveAbility" to clean up Enhanced Input Bindings.
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnGiveAbility")
	void K2_OnGiveAbility();

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnRemoveAbility")
	void K2_OnRemoveAbility();

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityEvent")
	void K2_OnAbilityEvent(const FGameplayEventData& EventData);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage", meta = (Categories = "AnimNotify", ForceInlineRow, TitleProperty="EventTag"))
	TArray<FRsAbilityEventInfo> AbilityEvents;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	TArray<UAnimMontage*> Montages;

	// Override in BP to select a montage. Defaults to a random one.
	UFUNCTION(BlueprintNativeEvent, Category = "RS|Mongage")
	UAnimMontage* SetMontageToPlay();
	
	UFUNCTION()
	void HandleMontageCompleted();

	UFUNCTION()
	void HandleMontageCancelled();

	UFUNCTION()
	void HandleAbilityEvent(FGameplayEventData EventData);

private:
	UPROPERTY(Transient)
	TObjectPtr<UAnimMontage> MontageToPlay;
	
	mutable FActiveGameplayEffectHandle CurrentCooldownHandle;
	mutable FGameplayTagContainer CurrentCooldownTags;
	int32 CurrentRechargeStacks = 0;
	FDelegateHandle RechargeDelegateHandle;
};
