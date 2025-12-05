// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"
#include "RsStaggerComponent.generated.h"

class URsStaggerSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGroggyEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRsStagger_AttributeChanged, URsStaggerComponent*, StaggerComponent, float, OldValue, float, NewValue, AActor*, Instigator);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RS_API URsStaggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsStaggerComponent();
	
	virtual void BeginPlay() override;

	void Initialize(UAbilitySystemComponent* AbilitySystemComponent);

	UPROPERTY(BlueprintAssignable)
	FRsStagger_AttributeChanged OnStaggerChanged;

	UPROPERTY(BlueprintAssignable)
	FOnGroggyEvent OnGroggyStarted;

	UFUNCTION(BlueprintCallable)
	float GetCurrentStagger();

	UFUNCTION(BlueprintCallable)
	float GetMaxStagger();

protected:
	void HandleAbilitySystemInitialized();
	
	void HandleStaggerChange(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	UPROPERTY(Transient)
	TObjectPtr<const URsStaggerSet> StaggerSet;

	UPROPERTY(ReplicatedUsing = OnRep_bIsGroggy)
	bool bIsGroggy = false;

	UFUNCTION()
	void OnRep_bIsGroggy(bool OldValue);
};
