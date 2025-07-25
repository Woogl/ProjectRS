// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCharacterBase.h"
#include "RsEnemyCharacter.generated.h"

/**
 * Base class to use for Server controlled Characters.
 * This class contains its own Ability System Component.
 */
UCLASS(Abstract, Blueprintable)
class RS_API ARsEnemyCharacter : public ARsCharacterBase
{
	GENERATED_BODY()
	
public:
	ARsEnemyCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
};
