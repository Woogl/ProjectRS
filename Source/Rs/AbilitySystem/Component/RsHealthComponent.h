// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"
#include "RsHealthComponent.generated.h"

class UAbilitySystemComponent;
class URsHealthSet;
struct FOnAttributeChangeData;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class RS_API URsHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsHealthComponent();

	void Initialize(UAbilitySystemComponent* AbilitySystemComponent);

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChange OnHealthChange;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

protected:
	void HandleHealthChange(const FOnAttributeChangeData& ChangeData);

	UPROPERTY()
	TObjectPtr<const URsHealthSet> HealthSet;

	UPROPERTY(ReplicatedUsing = OnRep_bIsDead)
	bool bIsDead = false;

	UFUNCTION()
	void OnRep_bIsDead(bool OldbIsDead);
};
