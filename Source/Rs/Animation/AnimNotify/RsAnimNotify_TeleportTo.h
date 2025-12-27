// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyBase.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotify_TeleportTo.generated.h"

class ARsCharacterBase;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_TeleportTo : public URsAnimNotifyBase
{
	GENERATED_BODY()

public:
	URsAnimNotify_TeleportTo();
	
	UPROPERTY(EditAnywhere)
	FVector TargetLocalOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	bool bLookTarget = false;
	
	UPROPERTY(EditAnywhere)
	FRsTargetingParams TargetingParams;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
