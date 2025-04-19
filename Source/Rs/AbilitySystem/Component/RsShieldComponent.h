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

	void Initialize(UAbilitySystemComponent* ASC);

	// Apply damage to each shield. The shield with shortest reamining time will hit first.
	void ApplyDamageToShields(float DamageAmount);

protected:
	void HandleShieldAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveHandle);
	void HandleShieldRemoved(const FGameplayEffectRemovalInfo& RemovalInfo);

	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveShieldHandles;

	UPROPERTY()
	TMap<FActiveGameplayEffectHandle, float> ActiveShieldAmounts;
};
