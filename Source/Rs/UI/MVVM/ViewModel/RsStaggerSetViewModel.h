// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetViewModelBase.h"
#include "RsStaggerSetViewModel.generated.h"

class URsStaggerSet;
struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS()
class RS_API URsStaggerSetViewModel : public URsAttributeSetViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsStaggerSetViewModel* CreateStaggerSetViewModel(URsStaggerSet* StaggerSet);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

public:
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetCurrentStagger() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	float GetMaxStagger() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	float GetStaggerDecay() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetStaggerPercent() const;
	
private:
	void MaxStaggerChanged(const FOnAttributeChangeData& Data);
	void CurrentStaggerChanged(const FOnAttributeChangeData& Data);
	void StaggerRegenChanged(const FOnAttributeChangeData& Data);
};
