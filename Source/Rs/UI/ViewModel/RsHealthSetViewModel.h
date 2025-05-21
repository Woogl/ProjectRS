// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsViewModelBase.h"
#include "RsHealthSetViewModel.generated.h"

class UAbilitySystemComponent;
struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS()
class RS_API URsHealthSetViewModel : public URsViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static URsHealthSetViewModel* CreateHealthSetViewModel(UAbilitySystemComponent* ASC);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

	TWeakObjectPtr<UAbilitySystemComponent> CachedModel;

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

	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetCurrentHealthText() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetMaxHealthText() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	bool IsDead() const;
	
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
