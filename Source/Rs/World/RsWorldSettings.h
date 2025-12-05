// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/WorldSettings.h"
#include "RsWorldSettings.generated.h"

class URsAbilitySystemComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class RS_API ARsWorldSettings : public AWorldSettings, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URsAbilitySystemComponent> AbilitySystemComponent;
	
public:
	ARsWorldSettings();
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};