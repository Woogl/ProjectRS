// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsViewModelBase.h"
#include "RsAttributeSetViewModelBase.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
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

	UPROPERTY(Transient)
	TWeakObjectPtr<UAbilitySystemComponent> ASC;
};
