// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_ActivateAbility.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_ActivateAbility : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(Categories="Ability"))
	FGameplayTag AbilityTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Condition")
	bool bHasHitTarget = false;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
