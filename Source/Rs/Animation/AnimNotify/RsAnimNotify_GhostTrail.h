// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_GhostTrail.generated.h"

class ARsGhostTrail;
/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_GhostTrail : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARsGhostTrail> GhostTrailClass;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY()
	TObjectPtr<ARsGhostTrail> SpawnedGhostTrail;

	void DestroyGhostTrail();
};
