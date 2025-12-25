// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "RsAnimNotify_ActivateAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_ActivateAbility : public URsAnimNotifyBase
{
	GENERATED_BODY()

public:
	URsAnimNotify_ActivateAbility();
	
	UPROPERTY(EditAnywhere, Category="AnimNotify", meta=(Categories="Ability"))
	FGameplayTagContainer AbilityTags;

	UPROPERTY(EditAnywhere, Category="AnimNotify")
	bool bCancelCurrentAbility = false;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
