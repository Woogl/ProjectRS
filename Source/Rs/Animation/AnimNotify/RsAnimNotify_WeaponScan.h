// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotify_WeaponScan.generated.h"

/**
 * 
 */
// TODO: Make it compatible with RsWeapon class
UCLASS()
class RS_API URsAnimNotify_WeaponScan : public UAnimNotify
{
	GENERATED_BODY()

public:
	URsAnimNotify_WeaponScan();
	
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

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
