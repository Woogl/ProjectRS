// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility_Attack.h"
#include "RsProjectile.generated.h"

class URsGameplayAbility_Attack;
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

	TWeakObjectPtr<URsGameplayAbility_Attack> OwningAbility;
	
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FRsDamageContext DamageContext;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayTag EventTag;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float MaxRange = 1000.f;

	// Default 0 or Minus value means infinite hit
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	int32 MaxHitCount = 1;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	bool bCannotHitFriend = true;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleBlock(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
