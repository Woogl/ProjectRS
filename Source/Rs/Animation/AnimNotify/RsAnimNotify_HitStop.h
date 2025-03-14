// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_HitStop.generated.h"

/**
 * 
 */
UCLASS(const, meta=(DisplayName="AN_HitStop"))
class RS_API URsAnimNotify_HitStop : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	float Duration = 0.2f;
	
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
