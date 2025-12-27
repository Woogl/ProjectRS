// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyBase.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotify_SpawnProjectile.generated.h"

class ARsProjectile;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_SpawnProjectile : public URsAnimNotifyBase
{
	GENERATED_BODY()

public:
	URsAnimNotify_SpawnProjectile();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARsProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	FRsTargetingParams TargetingParams;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions="GetSocketNames"))
	FName SpawnSocketName;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
