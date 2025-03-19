// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "Rs/Camera/LockOn/RsLockOnInterface.h"
#include "RsCharacterBase.generated.h"

class URsStaggerComponent;
class URsHealthComponent;
class URsAbilitySet;
class URsAbilitySystemComponent;

UENUM()
enum class ERsTeamId : uint8
{
	Player	= 0,
	Enemy	= 1,
	Neutral	= 255,
};

UCLASS(Abstract, NotBlueprintable)
class RS_API ARsCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface, public IRsLockOnInterface
{
	GENERATED_BODY()

public:
	ARsCharacterBase();
	
	// Implement the IAbilitySystemInterface. (This is used to find the Ability System Component.)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	URsAbilitySystemComponent* GetRsAbilitySystemComponent() const;

	// This event is fired after Ability System Component initialization is finished.
	UFUNCTION(BlueprintImplementableEvent)
	void PostInitializeAbilitySystem();

	// IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	// IRsTargetingInterface
	virtual bool IsLockableTarget_Implementation() const override;

protected:
	// Creates a pointer to the Ability System Component associated with this Character.
	// Player Characters will set this in OnRep_PlayerState() locally, and in OnPossessed() server side.
	// Non Player Characters will set this in its constructor.
	UPROPERTY()
	TObjectPtr<URsAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS")
	TObjectPtr<URsHealthComponent> HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS")
	TObjectPtr<URsStaggerComponent> StaggerComponent;

	// Data used to initialize the Ability System Component.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RS")
	TArray<URsAbilitySet*> AbilitySets;
	
	UPROPERTY(Replicated, EditAnywhere, Category = "RS")
	ERsTeamId TeamId = ERsTeamId::Neutral;
};
