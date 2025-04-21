// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "RsHealthSetViewModel.generated.h"

struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS()
class RS_API URsHealthSetViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static URsHealthSetViewModel* CreateHealthSetViewModel(AActor* Model);

protected:
	void Initialize();

public:
	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	float GetHealthRegen() const;
	float GetShield() const;
	
	void SetCurrentHealth(float NewCurrentHealth);
	void SetMaxHealth(float NewMaxHealth);
	void SetHealthRegen(float NewHealthRegen);
	void SetShield(float NewShield);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	float GetShieldPercent() const;
	
private:
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float CurrentHealth;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float MaxHealth;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float HealthRegen;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float Shield;

	void MaxHealthChanged(const FOnAttributeChangeData& Data);
	void CurrentHealthChanged(const FOnAttributeChangeData& Data);
	void HealthRegenChanged(const FOnAttributeChangeData& Data);
	void ShieldChanged(const FOnAttributeChangeData& Data);
};
