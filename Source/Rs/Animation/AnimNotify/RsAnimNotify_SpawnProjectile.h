﻿// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotify_Targeting.h"
#include "RsAnimNotify_SpawnProjectile.generated.h"

class ARsProjectile;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_SpawnProjectile : public URsAnimNotify_Targeting
{
	GENERATED_BODY()

public:
	URsAnimNotify_SpawnProjectile();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARsProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions="GetSocketNames"))
	FName SpawnSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFireAtLeastOne = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="AnimNotify"))
	FGameplayTag DamageEventTag;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	const float DefaultTargetDistance = 100.f;
};
