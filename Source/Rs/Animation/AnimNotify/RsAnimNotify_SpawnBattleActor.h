// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_SpawnBattleActor.generated.h"

class ARsBattleActor;
/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_SpawnBattleActor : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARsBattleActor> BattleActorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PositionOffset;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	TWeakObjectPtr<ARsBattleActor> WeakSpawnedActor;
};
