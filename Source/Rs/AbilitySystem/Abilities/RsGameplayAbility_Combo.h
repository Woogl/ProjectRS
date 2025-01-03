// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility.h"
#include "RsGameplayAbility_Combo.generated.h"

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
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS")
	TArray<TSubclassOf<URsGameplayAbility>> InnerAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS", meta = (Categories = "Event"))
	FGameplayTag ComboWindowStartTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS", meta = (Categories = "Event"))
	FGameplayTag ComboWindowEndTag;

	int32 IncreaseComboIndex();

	UFUNCTION()
	void HandleComboWindowStarted(FGameplayEventData Data);

	UFUNCTION()
	void HandleComboWindowEnded(FGameplayEventData Data);
	
	UFUNCTION()
	void HandleInputPressed(float TimeWaited);
	
	void HandleInnerAbilityEnded(const FAbilityEndedData& AbilityEndData);
	
private:
	UPROPERTY(VisibleAnywhere)
	int32 MaxComboIndex = 0;
	
	UPROPERTY(VisibleAnywhere)
	int32 CurrentComboIndex = 0;

	UPROPERTY()
	TArray<FGameplayAbilitySpec> InnerSpecs;
	
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> InnerSpecHandles;

	UPROPERTY()
	UAbilityTask_WaitInputPress* CachedPressTask = nullptr;
};
