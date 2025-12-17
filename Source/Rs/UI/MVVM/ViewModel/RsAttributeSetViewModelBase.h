// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "RsViewModelBase.h"
#include "RsAttributeSetViewModelBase.generated.h"

class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayAttribute, Attribute, float, OldValue, float, NewValue);

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAttributeSetViewModelBase : public URsViewModelBase
{
	GENERATED_BODY()

public:
	using ModelType = UAttributeSet;
	
protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;
	
	virtual void HandleAttributeChanged(const FOnAttributeChangeData& Data);

	TWeakObjectPtr<UAbilitySystemComponent> ASC;
	
private:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged;
};
