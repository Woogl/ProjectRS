// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"
#include "RsHealthComponent.generated.h"

class URsAbilitySystemComponent;
class URsHealthSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathEvent, AActor*, OwningActor);

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class RS_API URsHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsHealthComponent();
	
	virtual void BeginPlay() override;

	void Initialize(UAbilitySystemComponent* AbilitySystemComponent);
	void Deinitialize();

	// Apply damage to each barrier. The barrier with the shortest remaining time will hit first.
	void ApplyDamageToBarriers(UAbilitySystemComponent* AbilitySystemComponent, float DamageAmount);

	UPROPERTY(BlueprintAssignable)
	FOnDeathEvent OnDeathStarted;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	float GetBarrier();

protected:
	UPROPERTY(BlueprintAssignable)
	FRsAttributeChangedEvent OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FRsAttributeChangedEvent OnBarrierChanged;
	
	UPROPERTY(Transient)
	TObjectPtr<const URsHealthSet> HealthSet;
	
	void HandleAbilitySystemInitialized();
	
	void HandleHealthChange(const FOnAttributeChangeData& Data);
	void HandleBarrierChange(const FOnAttributeChangeData& Data);
	
	void HandleBarrierAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	void HandleBarrierRemove(const FGameplayEffectRemovalInfo& RemovalInfo, float Magnitude);
	void HandleBarrierBroke(const FActiveGameplayEffectHandle& BrokenBarrierHandle);
	
	UPROPERTY(ReplicatedUsing = OnRep_bIsDead)
	bool bIsDead = false;

	UFUNCTION()
	void OnRep_bIsDead(bool OldValue);
	
	UPROPERTY(Transient)
	TObjectPtr<URsAbilitySystemComponent> OwnerAbilitySystemComponent;
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveBarrierHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> RemovePendingBarrierHandles;
};
