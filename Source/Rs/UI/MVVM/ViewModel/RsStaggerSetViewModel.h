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
	using ModelType = URsStaggerSet;
	
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsStaggerSetViewModel* CreateStaggerSetViewModel(const URsStaggerSet* StaggerSet);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

public:
	float GetCurrentStagger() const;
	void SetCurrentStagger(float Value);
	
	float GetMaxStagger() const;
	void SetMaxStagger(float Value);
	
	float GetStaggerDecay() const;
	void SetStaggerDecay(float Value);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetStaggerPercent() const;
	
private:
	void MaxStaggerChanged(const FOnAttributeChangeData& Data);
	void CurrentStaggerChanged(const FOnAttributeChangeData& Data);
	void StaggerDecayChanged(const FOnAttributeChangeData& Data);
	
	UPROPERTY(FieldNotify, Getter, Setter, meta=(AllowPrivateAccess))
	float CurrentStagger = 0.f;
	
	UPROPERTY(FieldNotify, Getter, Setter, meta=(AllowPrivateAccess))
	float MaxStagger = 0.f;
	
	UPROPERTY(FieldNotify, Getter, Setter, meta=(AllowPrivateAccess))
	float StaggerDecay = 0.f;
};
