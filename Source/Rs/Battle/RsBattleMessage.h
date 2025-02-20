// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsBattleMessage.generated.h"

class ARsCharacterBase;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRsLinkSkillMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ARsCharacterBase* TriggeredCharacter;
};