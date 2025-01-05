// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "RsProjectile.generated.h"

UCLASS()
class RS_API ARsProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess))
	class UProjectileMovementComponent* ProjectileMovement;
	
public:	
	ARsProjectile();

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageSpecHandle;
};
