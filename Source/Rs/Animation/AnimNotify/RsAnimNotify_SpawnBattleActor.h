// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_SpawnBattleActor.generated.h"

class ARsBattleActor;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_SpawnBattleActor : public UAnimNotify
{
	GENERATED_BODY()

public:
	URsAnimNotify_SpawnBattleActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARsBattleActor> BattleActorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LocalPosition = FVector(150.f, 0.f, 0.f);
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	TWeakObjectPtr<ARsBattleActor> WeakSpawnedActor;
};
