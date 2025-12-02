// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"
#include "RsHealthComponent.generated.h"

class URsHealthSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathEvent, AActor*, OwningActor);

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class RS_API URsHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsHealthComponent();

	void Initialize(UAbilitySystemComponent* AbilitySystemComponent);

	// Apply damage to each barrier. The barrier with the shortest remaining time will hit first.
	void ApplyDamageToBarriers(UAbilitySystemComponent* AbilitySystemComponent, float DamageAmount);

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChange OnHealthChange;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChange OnBarrierChange;

	UPROPERTY(BlueprintAssignable)
	FOnDeathEvent OnDeathStarted;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	float GetBarrier();

protected:
	UPROPERTY()
	TObjectPtr<const URsHealthSet> HealthSet;
	
	void HandleHealthChange(const FOnAttributeChangeData& ChangeData);
	void HandleBarrierChange(const FOnAttributeChangeData& ChangeData);
	
	void HandleBarrierAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	void HandleBarrierRemove(const FGameplayEffectRemovalInfo& RemovalInfo, float Magnitude);
	void HandleBarrierBroke(const FActiveGameplayEffectHandle& BrokenBarrierHandle);
	
	UPROPERTY(ReplicatedUsing = OnRep_bIsDead)
	bool bIsDead = false;

	UPROPERTY(ReplicatedUsing = OnRep_bIsInitialized)
	bool bIsInitialized = false;

	UFUNCTION()
	void OnRep_bIsDead(bool OldValue);
	
	UFUNCTION()
	void OnRep_bIsInitialized(bool OldValue);
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveBarrierHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> RemovePendingBarrierHandles;
};
