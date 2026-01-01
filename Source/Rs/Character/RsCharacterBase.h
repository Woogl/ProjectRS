// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "Rs/Targeting/RsTargetingInterface.h"
#include "RsCharacterBase.generated.h"

class URsRagdollComponent;
class URsStaggerComponent;
class URsHealthComponent;
class URsAbilitySystemComponent;
class UBehaviorTree;

UENUM()
enum class ERsTeamId : uint8
{
	Player	= 0,
	Enemy	= 1,
	Neutral	= 255,
};

UCLASS(Abstract, NotBlueprintable)
class RS_API ARsCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public IGenericTeamAgentInterface, public IRsTargetingInterface
{
	GENERATED_BODY()

public:
	ARsCharacterBase(const FObjectInitializer& ObjectInitializer);
	
	// Implement the IAbilitySystemInterface. (This is used to find the Ability System Component.)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	URsAbilitySystemComponent* GetRsAbilitySystemComponent() const;

	//~ IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& OutTagContainer) const override;

	// IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	// IRsTargetingInterface
	virtual bool IsTargetable_Implementation() const override;

protected:
	// Creates a pointer to the Ability System Component associated with this Character.
	// Player Characters will set this in OnRep_PlayerState() locally, and in OnPossessed() server side.
	// Non Player Characters will set this in its constructor.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS")
	TObjectPtr<URsAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS")
	TObjectPtr<URsHealthComponent> HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS")
	TObjectPtr<URsStaggerComponent> StaggerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS")
	TObjectPtr<USkeletalMeshComponent> CharacterAppearance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS")
	TObjectPtr<URsRagdollComponent> RagdollComponent;
	
	UFUNCTION()
	virtual void HandleDeathStarted(AActor* OwningActor);

	UFUNCTION()
	virtual void HandleGroggyStarted(AActor* OwningActor);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "RS|UI", meta=(DisplayThumbnail="true", AllowedClasses="/Script/Engine.MaterialInterface,/Script/Engine.Texture2D"))
	TObjectPtr<UObject> Portrait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "RS|UI")
	FText CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "RS|UI")
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(Replicated, EditAnywhere, Category = "RS")
	ERsTeamId TeamId = ERsTeamId::Neutral;
};
