// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsProjectile.generated.h"

class URsGameplayAbility;
class UProjectileMovementComponent;
class UCapsuleComponent;

UENUM()
enum class ERsProjectileDirection : uint8
{
	SourceForward,
	SourceToTarget,
	SkyToTarget,
};

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
	
	void SetupDamage(URsGameplayAbility* OwningAbility, FGameplayTag DamageEventTag);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "RS")
	float MaxRange = 5000.f;

	// 0 or Minus value means infinite hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "RS")
	int32 MaxHitCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "RS")
	FRsTargetingFilter DamageFilter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "RS")
	ERsProjectileDirection Direction;

	// Relative spawn height when the direction is set to "SkyToTarget".
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true, EditCondition = "Direction == ERsProjectileDirection::SkyToTarget"), Category = "RS")
	float SpawnHeight = 1000.f;

	// Relative distance when targeting failed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true, EditCondition = "Direction == ERsProjectileDirection::SkyToTarget"), Category = "RS")
	float FallbackSpawnDistance = 300.f;

protected:
	virtual void BeginPlay() override;

	TWeakObjectPtr<URsGameplayAbility> OwningAbility;
	
	UPROPERTY()
	FGameplayTag DamageEvent;
	
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleBlock(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
