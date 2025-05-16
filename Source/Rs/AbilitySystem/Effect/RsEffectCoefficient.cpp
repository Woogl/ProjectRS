// Copyright 2024 Team BH.


#include "RsEffectCoefficient.h"

#include "GameplayEffect.h"
#include "Rs/RsGameplayTags.h"

bool FRsEffectCoefficient::IsValid() const
{
	if (EffectClass == nullptr)
	{
		return false;
	}
	if (Coefficients.IsEmpty())
	{
		return false;
	}
	return true;
}

FRsEffectCoefficient::FRsEffectCoefficient(TSubclassOf<UGameplayEffect> InEffectClass, const TMap<FGameplayTag, float>& InCoefficients)
	: EffectClass(InEffectClass), Coefficients(InCoefficients)
{
}

FRsEffectCoefficient::FRsEffectCoefficient(TSubclassOf<UGameplayEffect> InEffectClass, float Magnitude)
	: EffectClass(InEffectClass)
{
	Coefficients.Add(RsGameplayTags::MANUAL_MAGNITUDE, Magnitude);
}

FRsEffectCoefficient::FRsEffectCoefficient(TSubclassOf<UGameplayEffect> InEffectClass, float Magnitude, float Duration)
	: EffectClass(InEffectClass)
{
	Coefficients.Add(RsGameplayTags::MANUAL_MAGNITUDE, Magnitude);
	Coefficients.Add(RsGameplayTags::MANUAL_DURATION, Duration);
}
