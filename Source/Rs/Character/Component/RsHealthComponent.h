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

	// Apply damage to each shield. The shield with shortest reamining time will hit first.
	void ApplyDamageToShields(UAbilitySystemComponent* AbilitySystemComponent, float DamageAmount);

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChange OnHealthChange;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChange OnShieldChange;

	UPROPERTY(BlueprintAssignable)
	FOnDeathEvent OnDeathStarted;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	float GetShield();

protected:
	UPROPERTY()
	TObjectPtr<const URsHealthSet> HealthSet;
	
	void HandleHealthChange(const FOnAttributeChangeData& ChangeData);
	void HandleShieldChange(const FOnAttributeChangeData& ChangeData);
	
	void HandleShieldAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	void HandleShieldRemove(const FGameplayEffectRemovalInfo& RemovalInfo, float Magnitude);
	void HandleShieldBroke(const FActiveGameplayEffectHandle& BrokenShieldHandle);

	UPROPERTY(ReplicatedUsing = OnRep_bIsDead)
	bool bIsDead = false;

	UFUNCTION()
	void OnRep_bIsDead(bool OldValue);
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveShieldHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> RemovePendingShieldHandles;
};
