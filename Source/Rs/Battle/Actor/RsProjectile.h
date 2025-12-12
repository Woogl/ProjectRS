// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsProjectile.generated.h"

class UArrowComponent;
class USphereComponent;
class UGameplayEffect;
class UProjectileMovementComponent;

UCLASS(Abstract)
class RS_API ARsProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collision;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> Arrow;

public:
	// Apply when collision overlapped or blocked.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RS")
	TSubclassOf<UGameplayEffect> Effect;
	
	// Apply when collision overlapped or blocked.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RS", meta = (RowType ="RsEffectTableRowBase"))
	FDataTableRowHandle EffectTableRow;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RS")
	FRsTargetingFilter TargetFilter;

	// Limit on effect application counts (0 means no limit).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RS")
	int32 MaxEffectApplyCounts = 1;
	
	UPROPERTY(Transient)
	TObjectPtr<UGameplayAbility> OwningAbility;
	
public:	
	ARsProjectile();
	
	bool ApplyEffect(AActor* Target, const FHitResult& HitResult);

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleBlock(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	TArray<FGameplayEffectSpecHandle> EffectSpecs;
	
	UPROPERTY(Transient)
	TObjectPtr<UAbilitySystemComponent> InstigatorASC;
};
