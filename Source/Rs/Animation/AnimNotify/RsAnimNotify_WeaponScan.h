// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyState_WeaponTrace.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotify_WeaponScan.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_WeaponScan : public UAnimNotify
{
	GENERATED_BODY()

public:
	URsAnimNotify_WeaponScan();
	
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

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	TWeakObjectPtr<USceneComponent> WeaponComponent;
	
	FTransform GetWeaponTransform() const;
};
