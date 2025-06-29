﻿// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_DespawnBattleActors.generated.h"

class ARsBattleActor;
/**
 * Despawn all actors of class managed by RsBattleActorComponent.
 */
UCLASS()
class RS_API URsAnimNotify_DespawnBattleActors : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARsBattleActor> BattleActorClass;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
