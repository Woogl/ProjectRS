// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Actor.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsWeapon.generated.h"

/**
 * Base class for weapon actor.
 * Requires a Box, Capsule, or Sphere component as the root component.
 */
UCLASS(Abstract)
class RS_API ARsWeapon : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	ARsWeapon();
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	static const FName DefaultWeaponID;
	FName GetWeaponID() const;
	
	UPrimitiveComponent* GetRootPrimitive() const;
	FRsTargetingShape GetWeaponShape() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="RS")
	FName WeaponID;
	
	virtual void BeginPlay() override;
};
