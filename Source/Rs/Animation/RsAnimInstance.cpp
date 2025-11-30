// Copyright 2025 Team BH.


#include "RsAnimInstance.h"

#include "AbilitySystemGlobals.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"

float URsAnimInstance::Montage_PlayInternal(UAnimMontage* MontageToPlay, const FMontageBlendSettings& BlendInSettings, float InPlayRate, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages)
{
	if (APawn* Pawn = TryGetPawnOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn))
		{
			float ActionSpeed = URsAbilitySystemLibrary::GetNumericAttributeByTag(ASC, RsGameplayTags::STAT_ATS);
			InPlayRate *= ActionSpeed;
		}
	}
	return Super::Montage_PlayInternal(MontageToPlay, BlendInSettings, InPlayRate, ReturnValueType, InTimeToStartMontageAt, bStopAllMontages);
}
