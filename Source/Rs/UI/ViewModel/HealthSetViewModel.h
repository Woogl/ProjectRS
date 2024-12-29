// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "HealthSetViewModel.generated.h"

struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS()
class RS_API UHealthSetViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UHealthSetViewModel* CreateHealthSetViewModel(AActor* Model);

	void Initialize();

	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	float GetHealthRegen() const;
	
	void SetCurrentHealth(float NewCurrentHealth);
	void SetMaxHealth(float NewMaxHealth);
	void SetHealthRegen(float NewHealthRegen);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetHealthPercent() const;
	
private:
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float CurrentHealth;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float MaxHealth;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float HealthRegen;

	void MaxHealthChanged(const FOnAttributeChangeData& Data);
	void CurrentHealthChanged(const FOnAttributeChangeData& Data);
	void HealthRegenChanged(const FOnAttributeChangeData& Data);
};
