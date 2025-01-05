// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility.h"
#include "RsGameplayAbility_Combo.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_WaitInputPress;
/**
 * Composite Ability that can activate inner abilities.
 */
UCLASS()
class RS_API URsGameplayAbility_Combo : public URsGameplayAbility
{
	GENERATED_BODY()

public:
	URsGameplayAbility_Combo();
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	void ActivateInnerAbility();
	
	int32 IncrementComboIndex();
	void ResetComboIndex();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS")
	TArray<TSubclassOf<URsGameplayAbility>> InnerAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS", meta = (Categories = "Event"))
	FGameplayTag ComboWindowStartTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS", meta = (Categories = "Event"))
	FGameplayTag ComboWindowEndTag;
	
	UPROPERTY()
	TArray<FGameplayAbilitySpec> InnerSpecs;
	
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> InnerSpecHandles;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> InnerSpecHandlesActivating;
	
	UFUNCTION()
	void HandleComboWindowStarted(FGameplayEventData Data);

	UFUNCTION()
	void HandleComboWindowEnded(FGameplayEventData Data);
	
	UFUNCTION()
	void HandleInputPressed(float TimeWaited);

	void HandleInnerAbilityActivated(UGameplayAbility* ActivatedAbility);
	void HandleInnerAbilityEnded(const FAbilityEndedData& AbilityEndData);
	
private:
	UPROPERTY(Transient)
	UAbilityTask_WaitInputPress* InputPressTask = nullptr;
	
	UPROPERTY(Transient)
	int32 MaxComboIndex = 0;

	UPROPERTY(Transient)
	int32 CurrentComboIndex = 0;
};
