// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotify_TargetingBase.h"
#include "RsAnimNotify_EventToSelf.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_EventToSelf : public URsAnimNotify_TargetingBase
{
	GENERATED_BODY()

public:
	URsAnimNotify_EventToSelf();
	
#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif // WITH_EDITOR

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="AnimNotify"))
	FGameplayTag EventTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAlwaysSuccess = true;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
