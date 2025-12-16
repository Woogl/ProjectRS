// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyState_TargetingBase.h"
#include "RsAnimNotifyState_EventToSelf.generated.h"

class URsGameplayAbility;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_EventToSelf : public URsAnimNotifyState_TargetingBase
{
	GENERATED_BODY()
	
public:
	URsAnimNotifyState_EventToSelf();
	
#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif // WITH_EDITOR
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="AnimNotify"))
	FGameplayTag EventTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAlwaysSuccess = true;
	
	UPROPERTY(Transient)
	TArray<AActor*> Targets;
	
	UPROPERTY(Transient)
	TObjectPtr<URsGameplayAbility> CurrentAbility;
};
