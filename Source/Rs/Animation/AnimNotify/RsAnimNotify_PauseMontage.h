// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_PauseMontage.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_PauseMontage : public UAnimNotify
{
	GENERATED_BODY()

public:
	URsAnimNotify_PauseMontage();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	float Duration = 0.25f;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

#if WITH_EDITOR
	void ResumeMontage();
#endif // WITH_EDITOR

#if WITH_EDITORONLY_DATA
	TWeakObjectPtr<USkeletalMeshComponent> CachedMeshComp;
	FTimerHandle Timer;
#endif // WITH_EDITORONLY_DATA
};
