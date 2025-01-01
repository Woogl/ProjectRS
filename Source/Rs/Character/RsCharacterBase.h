// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "Rs/AbilitySystem/Data/RsAbilitySet.h"
#include "RsCharacterBase.generated.h"

class URsAbilitySystemComponent;

UCLASS(Abstract, NotBlueprintable)
class RS_API ARsCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Implement the IAbilitySystemInterface. (This is used to find the Ability System Component.)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	URsAbilitySystemComponent* GetRsAbilitySystemComponent() const;
	
	// This event is fired after Ability System Component initialization is finished.
    UFUNCTION(BlueprintImplementableEvent)
    void PostInitializeAbilitySystem();

	// IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override { TeamID = InTeamID; }
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }

protected:
	// Creates a pointer to the Ability System Component associated with this Character.
	// Player Characters will set this in OnRep_PlayerState() locally, and in OnPossessed() server side.
	// Non Player Characters will set this in its constructor.
	UPROPERTY()
	TObjectPtr<URsAbilitySystemComponent> AbilitySystemComponent;

	// Data used to initialize the Ability System Component.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RS")
	TObjectPtr<URsAbilitySet> AbilitySet;

	UPROPERTY(Replicated, EditAnywhere, Category = "RS")
	FGenericTeamId TeamID = FGenericTeamId::NoTeam;
};
