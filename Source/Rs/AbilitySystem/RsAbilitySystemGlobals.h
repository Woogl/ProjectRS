// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "RsAbilitySystemGlobals.generated.h"

#define SET_BY_CALLER_PROPERTY(Spec, PropertyName) \
URsAbilitySystemGlobals::SetByCallerProperty(Spec, GET_MEMBER_NAME_CHECKED(ThisClass, PropertyName), PropertyName)

/**
 * 
 */
UCLASS()
class RS_API URsAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	/** Should allocate a project specific GameplayEffectContext struct. Caller is responsible for deallocation */
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

	static void SetByCallerProperty(const FGameplayEffectSpecHandle& SpecHandle, FName SetByCallerName, float Magnitude);
};
