// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsViewModelBase.h"
#include "RsStaggerSetViewModel.generated.h"

class UAbilitySystemComponent;
struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS()
class RS_API URsStaggerSetViewModel : public URsViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsStaggerSetViewModel* CreateStaggerSetViewModel(UAbilitySystemComponent* ASC);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

	TWeakObjectPtr<UAbilitySystemComponent> CachedModel;

public:
	float GetCurrentStagger() const;
	float GetMaxStagger() const;
	float GetStaggerDecay() const;
	
	void SetCurrentStagger(float NewCurrentStagger);
	void SetMaxStagger(float NewMaxStagger);
	void SetStaggerDecay(float NewStaggerDecay);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetStaggerPercent() const;
	
private:
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float CurrentStagger;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float MaxStagger;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float StaggerDecay;

	void MaxStaggerChanged(const FOnAttributeChangeData& Data);
	void CurrentStaggerChanged(const FOnAttributeChangeData& Data);
	void StaggerRegenChanged(const FOnAttributeChangeData& Data);
};
