// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetViewModelBase.h"
#include "RsEnergySetViewModel.generated.h"

class URsEnergySet;
struct FOnAttributeChangeData;
/**
 * 
 */
UCLASS()
class RS_API URsEnergySetViewModel : public URsAttributeSetViewModelBase
{
	GENERATED_BODY()

public:
	using ModelType = URsEnergySet;
	
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsEnergySetViewModel* CreateEnergySetViewModel(const URsEnergySet* EnergySet);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

public:
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetCurrentEnergy() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	float GetMaxEnergy() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetEnergyPercent() const;
	
private:
	void MaxEnergyChanged(const FOnAttributeChangeData& Data);
	void CurrentEnergyChanged(const FOnAttributeChangeData& Data);
};
