// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotify_TargetingBase.h"
#include "RsAnimNotify_HitScan.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_HitScan : public URsAnimNotify_TargetingBase
{
	GENERATED_BODY()

public:
	URsAnimNotify_HitScan();
	
	virtual FString GetNotifyName_Implementation() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="AnimNotify"))
	FGameplayTag EventTag;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
