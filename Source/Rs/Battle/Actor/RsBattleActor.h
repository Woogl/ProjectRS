// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Actor.h"
#include "RsBattleActor.generated.h"

class URsAbilitySet;
enum class ERsTeamId : uint8;
class URsAbilitySystemComponent;

UCLASS()
class RS_API ARsBattleActor : public AActor, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ARsBattleActor();

protected:
	virtual void BeginPlay() override;

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& OutTagContainer) const override;

	// IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<URsAbilitySystemComponent> AbilitySystemComponent;

	// Data used to initialize the Ability System Component.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RS")
	TArray<URsAbilitySet*> AbilitySets;

	UPROPERTY(Replicated, EditAnywhere, Category = "RS")
	ERsTeamId TeamId;

public:
	virtual void Tick(float DeltaTime) override;
};
