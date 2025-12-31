// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Actor.h"
#include "RsWeapon.generated.h"

UCLASS()
class RS_API ARsWeapon : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	ARsWeapon();
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	static const FName DefaultWeaponID;
	FName GetWeaponID() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="RS")
	FName WeaponID;
	
	virtual void BeginPlay() override;
};
