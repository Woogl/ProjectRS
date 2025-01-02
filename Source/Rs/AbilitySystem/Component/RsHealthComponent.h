// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RsHealthComponent.generated.h"

class UAbilitySystemComponent;
class URsHealthSet;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHealthAttributeChanged, float, OldValue, float, NewValue, AActor*, Instigator);

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class RS_API URsHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsHealthComponent();

	void Initialize(UAbilitySystemComponent* AbilitySystemComponent);

	UPROPERTY(BlueprintAssignable)
	FHealthAttributeChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

protected:
	void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);

	UPROPERTY()
	TObjectPtr<const URsHealthSet> HealthSet;
};
