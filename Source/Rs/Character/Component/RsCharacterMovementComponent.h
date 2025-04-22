// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RsCharacterMovementComponent.generated.h"

class URsMovementSet;
/**
 * 
 */
UCLASS()
class RS_API URsCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	virtual float GetMaxSpeed() const override;

protected:
	UPROPERTY()
	TObjectPtr<const URsMovementSet> MovementSet = nullptr;
};
