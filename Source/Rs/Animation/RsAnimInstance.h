// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RsAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual float Montage_PlayInternal(UAnimMontage* MontageToPlay, const FMontageBlendSettings& BlendInSettings, float InPlayRate = 1.f, EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength, float InTimeToStartMontageAt = 0.f, bool bStopAllMontages = true) override;
};
