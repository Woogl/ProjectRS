// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Components/ActorComponent.h"
#include "RsShieldComponent.generated.h"

class UAbilitySystemComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RS_API URsShieldComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsShieldComponent();

	void Initialize(UAbilitySystemComponent* AbilitySystemComponent);

	// Apply damage to each shield. The shield with shortest reamining time will hit first.
	void ApplyDamageToShields(float DamageAmount);

protected:
	void HandleShieldAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	void HandleShieldRemove(const FGameplayEffectRemovalInfo& RemovalInfo,float Magnitude);
	void HandleShieldBroke(const FActiveGameplayEffectHandle& BrokenShieldHandle);

	TWeakObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	FGameplayAttribute ShieldAttribute;
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveShieldHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> RemovePendingShieldHandles;
};
