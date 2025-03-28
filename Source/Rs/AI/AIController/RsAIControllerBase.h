// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "RsAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class RS_API ARsAIControllerBase : public AAIController, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerception;*/

public:
	ARsAIControllerBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// IGenericTeamAgentInterface
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void HandlePlayerControllingPawnChanged(APawn* OldPawn, APawn* NewPawn);

private:
	UPROPERTY()
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
