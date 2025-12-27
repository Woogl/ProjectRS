// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Rs/Battle/Actor/RsWeapon.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotifyState_WeaponTrace.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_WeaponTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_WeaponTrace();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	ERsWeaponRecognitionMethod WeaponRecognition = ERsWeaponRecognitionMethod::ByMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FName WeaponTag = FName("Weapon");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FTransform Offset = FTransform::Identity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	ERsTargetingShapeType ShapeType = ERsTargetingShapeType::Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingFilter Filter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingSorter Sorter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="AnimNotify"))
	FGameplayTag EventTag;
	
	// Maximum number of traces per tick. It will prevents gaps that could cause missed hits.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSubsteps = 5;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	FTransform GetWeaponTransform() const;
	
	TWeakObjectPtr<USceneComponent> WeaponComponent;

	// Ignore already hit actors
	UPROPERTY()
	TArray<AActor*> HitTargets;

	UPROPERTY()
	FTransform LastTransform;

	UPROPERTY()
	FRsTargetingShape Shape;
};
