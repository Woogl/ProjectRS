// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotifyState_WeaponTrace.generated.h"

/**
 * 
 */
// TODO: Make it compatible with RsWeapon class
UCLASS()
class RS_API URsAnimNotifyState_WeaponTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_WeaponTrace();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FName ComponentTag = FName("Weapon");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FVector Scale = FVector::OneVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	ERsTargetingShape ShapeType = ERsTargetingShape::Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingFilter Filter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingSorter Sorter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="AnimNotify"))
	FGameplayTag DamageEvent;
	
	// Maximum number of traces per tick. It will prevents gaps that could cause missed hits.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSubsteps = 4;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	FTransform GetWeaponTransform() const;
	
	TWeakObjectPtr<UMeshComponent> WeaponComponent;

	// Ignore already hit actors
	UPROPERTY()
	TArray<AActor*> HitTargets;

	UPROPERTY()
	FTransform LastTransform;

	UPROPERTY()
	FRsTargetingCollision Collision;
};
