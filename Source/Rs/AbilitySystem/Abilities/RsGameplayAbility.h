// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RsGameplayAbility.generated.h"

class UGameplayEffect;
class UInputAction;
class ARsCharacterBase;

UENUM()
enum class ERsCooldownApplyTiming : uint8
{
	OnActivate,
	OnEnd
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
	UPROPERTY(EditDefaultsOnly, Category = "RS|Activation")
	bool bActivateOnGranted = false;

	// Which Input Action to bind the activation event to.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RS|Activation")
	TObjectPtr<UInputAction> ActivationInputAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RS|Effect", meta = (Categories = "AnimNotify", ForceInlineRow))
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> EffectMap;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RS|Effect", meta = (Categories = "AnimNotify", ForceInlineRow, RowType="RsEffectTableRowBase"))
	TMap<FGameplayTag, FDataTableRowHandle> EffectMapDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RS|Animation")
	TArray<UAnimMontage*> Montages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "RS|UI", meta=(DisplayThumbnail="true", AllowedClasses="/Script/Engine.MaterialInterface,/Script/Engine.Texture2D"))
	TObjectPtr<UObject> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "RS|UI")
	FText AbilityName = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "RS|UI")
	FText Description = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooldowns", meta = (Categories = "Cooldown"))
	FGameplayTag CooldownTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooldowns")
	float CooldownDuration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooldowns")
	ERsCooldownApplyTiming CooldownApplyTiming = ERsCooldownApplyTiming::OnActivate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooldowns", meta = (ClampMin = "0"))
	int32 MaxRechargeStacks = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Costs", meta = (ClampMin = "0"))
	float CostAmount = 0.f;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRechargeStacksChanged, int32 NewStacks);
	FOnRechargeStacksChanged OnRechargeStacksChanged;
	
	UFUNCTION(BlueprintCallable)
	ARsCharacterBase* GetAvatarCharacter() const;

	UFUNCTION(BlueprintCallable)
	virtual AController* GetController() const;

	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	UFUNCTION(BlueprintCallable, Category = "Cooldowns")
	void ModifyCooldownRemaining(float TimeDiff);

	UFUNCTION(BlueprintCallable, Category = "Cooldowns")
	void SetCooldownRemaining(float NewRemaining);

	UFUNCTION(BlueprintPure, Category = "Cooldowns")
	int32 GetCurrentRechargeStacks() const;
	
	UFUNCTION(BlueprintCallable, Category = "Cooldowns")
	void ModifyCurrentRechargeStacks(int32 Diff);

	UFUNCTION(BlueprintCallable, Category = "Cooldowns")
	void SetCurrentRechargeStacks(int32 NewStacks);
	
	// Called to bind Input Pressed and Input Released events to the Avatar Actor's Enhanced Input Component if it is reachable.
	void SetupEnhancedInputBindings(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	// Clear the bindings from the Enhanced Input Component.
	void TeardownEnhancedInputBindings(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);
	
	// Only effect that has duration can revert.
	void RevertGameplayEvent(FGameplayTag EventTag);

protected:
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
	
	UFUNCTION(BlueprintPure, Category = "RS")
	int32 GetDeterministicRandomNumber(int32 Min, int32 Max) const;

	// Override in BP to select a montage. Defaults to a random one.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "RS|Animation")
	UAnimMontage* SelectMontageToPlay() const;
	
	UFUNCTION()
	void HandleMontageCompleted();

	UFUNCTION()
	void HandleMontageCancelled();

	UFUNCTION()
	void HandleGameplayEvent(FGameplayEventData EventData);
	
	FGameplayEffectSpecHandle MakeOutgoingTableEffect(const FDataTableRowHandle* EffectTableRow, UAbilitySystemComponent* ASC, FGameplayEffectContextHandle EffectContext) const;

private:
	UPROPERTY(Transient)
	TObjectPtr<UAnimMontage> MontageToPlay;
	
	TMap<FGameplayTag, FActiveGameplayEffectHandle> EventEffectHandles;
	mutable FActiveGameplayEffectHandle CurrentCooldownHandle;
	mutable FGameplayTagContainer CurrentCooldownTags;
	int32 CurrentRechargeStacks = 0;
	FDelegateHandle RechargeDelegateHandle;
};
