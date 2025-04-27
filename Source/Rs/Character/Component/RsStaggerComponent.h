// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"
#include "RsStaggerComponent.generated.h"

class URsStaggerSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGroggyEvent, AActor*, OwningActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RS_API URsStaggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsStaggerComponent();

	void Initialize(UAbilitySystemComponent* AbilitySystemComponent);

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChange OnStaggerChange;

	UPROPERTY(BlueprintAssignable)
	FOnGroggyEvent OnGroggyEvent;

	UFUNCTION(BlueprintCallable)
	float GetCurrentStagger();

	UFUNCTION(BlueprintCallable)
	float GetMaxStagger();

protected:
	void HandleStaggerChange(const FOnAttributeChangeData& ChangeData);

	UPROPERTY()
	TObjectPtr<const URsStaggerSet> StaggerSet;

	UPROPERTY(ReplicatedUsing = OnRep_bIsGroggy)
	bool bIsGroggy = false;

	UFUNCTION()
	void OnRep_bIsGroggy(bool OldValue);
};
