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
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsHealthSetViewModel* CreateHealthSetViewModel(URsHealthSet* HealthSet);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

public:
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetCurrentHealth() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	float GetMaxHealth() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetHealthRegen() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	float GetShield() const;
	
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

	UFUNCTION(FieldNotify, BlueprintPure)
	FLinearColor GetColorByDeathState() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FLinearColor GetColorByHealthPercent() const;
	
private:
	void MaxHealthChanged(const FOnAttributeChangeData& Data);
	void CurrentHealthChanged(const FOnAttributeChangeData& Data);
	void HealthRegenChanged(const FOnAttributeChangeData& Data);
	void ShieldChanged(const FOnAttributeChangeData& Data);
};
