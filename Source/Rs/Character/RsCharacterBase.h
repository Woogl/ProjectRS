// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Rs/AbilitySystem/Data/RsAbilitySystemDataAsset.h"
#include "RsCharacterBase.generated.h"

class URsAbilitySystemComponent;

UCLASS(Abstract, NotBlueprintable)
class RS_API ARsCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Implement the IAbilitySystemInterface. (This is used to find the Ability System Component.)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	URsAbilitySystemComponent* GetRsAbilitySystemComponent() const;
	
	// This event is fired after Ability System Component initialization is finished.
    UFUNCTION(BlueprintImplementableEvent)
    void PostInitializeAbilitySystem();

protected:
	// Creates a pointer to the Ability System Component associated with this Character.
	// Player Characters will set this in OnRep_PlayerState() locally, and in OnPossessed() server side.
	// Non Player Characters will set this in its constructor.
	UPROPERTY()
	TObjectPtr<URsAbilitySystemComponent> AbilitySystemComponent;

	// Data used to initialize the Ability System Component.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ability System")
	TObjectPtr<URsAbilitySystemDataAsset> AbilitySystemData;
};
