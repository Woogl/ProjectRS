// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCharacterBase.h"
#include "RsEnemyCharacter.generated.h"

/**
 * Base class to use for Server controlled Characters.
 * This class contains its own Ability System Component.
 */
UCLASS(Blueprintable)
class RS_API ARsEnemyCharacter : public ARsCharacterBase
{
	GENERATED_BODY()

public:
	ARsEnemyCharacter();

protected:
	// Override "BeginPlay" to initialize the Ability System Component.
	// Note: The Ability System Component is created in the class constructor, so it should always be valid at this point.
	virtual void BeginPlay() override;
};
