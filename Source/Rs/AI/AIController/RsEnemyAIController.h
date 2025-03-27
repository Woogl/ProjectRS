// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAIControllerBase.h"
#include "RsEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class RS_API ARsEnemyAIController : public ARsAIControllerBase
{
	GENERATED_BODY()
	
public:
	ARsEnemyAIController();
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
};