// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetViewModelBase.h"
#include "RsHealthSetViewModel.generated.h"

class URsHealthSet;
struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS()
class RS_API URsHealthSetViewModel : public URsAttributeSetViewModelBase
{
	GENERATED_BODY()

public:
	using ModelType = URsHealthSet;
	
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsHealthSetViewModel* CreateHealthSetViewModel(const URsHealthSet* HealthSet);

protected:
	virtual void Initialize() override;
	virtual void HandleAttributeChanged(const FOnAttributeChangeData& Data) override;

public:
	float GetCurrentHealth() const;
	void SetCurrentHealth(float Value);
	
	float GetMaxHealth() const;
	void SetMaxHealth(float Value);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetCurrentHealthText() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetMaxHealthText() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetHealthPercent() const;
	
	float GetBarrier() const;
	void SetBarrier(float Value);

	UFUNCTION(FieldNotify, BlueprintPure)
	float GetBarrierPercent() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	bool IsDead() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FLinearColor GetPortraitColor() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FLinearColor GetHealthBarColor() const;
	
private:
	UPROPERTY(FieldNotify, Getter, Setter, meta=(AllowPrivateAccess))
	float CurrentHealth = 0.f;
	
	UPROPERTY(FieldNotify, Getter, Setter, meta=(AllowPrivateAccess))
	float MaxHealth = 0.f;
	
	UPROPERTY(FieldNotify, Getter, Setter, meta=(AllowPrivateAccess))
	float Barrier = 0.f;
};
