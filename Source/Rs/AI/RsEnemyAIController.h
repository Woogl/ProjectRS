// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RsEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class RS_API ARsEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerception;
	
public:
	ARsEnemyAIController();
	
	// IGenericTeamAgentInterface
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTree;
};