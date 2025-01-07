// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RsWeapon.generated.h"

UCLASS()
class RS_API ARsWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ARsWeapon();

protected:
	virtual void BeginPlay() override;
};
