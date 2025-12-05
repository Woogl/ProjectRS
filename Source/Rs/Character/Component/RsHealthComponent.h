// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"
#include "RsHealthComponent.generated.h"

class URsHealthSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRsHealth_AttributeChanged, URsHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class RS_API URsHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsHealthComponent();
	
	virtual void BeginPlay() override;

	void Initialize(UAbilitySystemComponent* AbilitySystemComponent);

	// Apply damage to each barrier. The barrier with the shortest remaining time will hit first.
	void ApplyDamageToBarriers(UAbilitySystemComponent* AbilitySystemComponent, float DamageAmount);

	UPROPERTY(BlueprintAssignable)
	FRsHealth_AttributeChanged OnCurrentHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FRsHealth_AttributeChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FRsHealth_AttributeChanged OnBarrierChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDeathEvent OnDeathStarted;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	float GetBarrier();

protected:
	UPROPERTY(Transient)
	TObjectPtr<const URsHealthSet> HealthSet;
	
	void HandleAbilitySystemInitialized();
	
	void HandleCurrentHealthChange(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	void HandleMaxHealthChange(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	void HandleBarrierChange(const FOnAttributeChangeData& ChangeData);
	
	void HandleBarrierAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	void HandleBarrierRemove(const FGameplayEffectRemovalInfo& RemovalInfo, float Magnitude);
	void HandleBarrierBroke(const FActiveGameplayEffectHandle& BrokenBarrierHandle);
	
	UPROPERTY(ReplicatedUsing = OnRep_bIsDead)
	bool bIsDead = false;

	UFUNCTION()
	void OnRep_bIsDead(bool OldValue);
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveBarrierHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> RemovePendingBarrierHandles;
};
