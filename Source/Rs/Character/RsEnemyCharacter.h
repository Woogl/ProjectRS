// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCharacterBase.h"
#include "RsEnemyCharacter.generated.h"

class URsNameplateComponent;
/**
 * Base class to use for Server controlled Characters.
 */
UCLASS(Abstract, Blueprintable)
class RS_API ARsEnemyCharacter : public ARsCharacterBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS")
	TObjectPtr<URsNameplateComponent> NameplateComponent;
	
public:
	ARsEnemyCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
};
