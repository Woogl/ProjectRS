// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAIControllerBase.h"
#include "RsFriendlyAIController.generated.h"

/**
 * 
 */
UCLASS()
class RS_API ARsFriendlyAIController : public ARsAIControllerBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARsFriendlyAIController();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
};
