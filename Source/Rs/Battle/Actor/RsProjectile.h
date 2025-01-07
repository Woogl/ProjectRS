// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "RsProjectile.generated.h"

class UProjectileMovementComponent;
class UCapsuleComponent;

UCLASS()
class RS_API ARsProjectile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCapsuleComponent* Capsule;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
	
public:	
	ARsProjectile();

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageSpecHandle;

	// Minus value means infinite hit
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	int32 MaxHitCount = 1;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bCannotHitFriend = true;

protected:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleBlock(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
