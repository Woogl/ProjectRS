// Copyright 2025 Team BH.


#include "RsAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Attributes/RsSpeedSet.h"

float URsAnimInstance::Montage_PlayInternal(UAnimMontage* MontageToPlay, const FMontageBlendSettings& BlendInSettings, float InPlayRate, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages)
{
	if (APawn* Pawn = TryGetPawnOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn))
		{
			float ActionSpeed = ASC->GetNumericAttribute(URsSpeedSet::GetActionSpeedAttribute());
			InPlayRate *= ActionSpeed;
		}
	}
	return Super::Montage_PlayInternal(MontageToPlay, BlendInSettings, InPlayRate, ReturnValueType, InTimeToStartMontageAt, bStopAllMontages);
}
