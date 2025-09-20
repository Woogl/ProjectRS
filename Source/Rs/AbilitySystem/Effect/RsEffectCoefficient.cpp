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

FRsEffectCoefficient::FRsEffectCoefficient(TSubclassOf<UGameplayEffect> EffectClass, const TArray<FRsStatCoefficient>& Coefficients)
	: EffectClass(EffectClass), Coefficients(Coefficients)
{
}

FRsEffectCoefficient::FRsEffectCoefficient(TSubclassOf<UGameplayEffect> EffectClass, float Magnitude)
	: EffectClass(EffectClass)
{
	FRsStatCoefficient StatCoefficient;
	StatCoefficient.StatCoefficients.Add(RsGameplayTags::MANUAL, Magnitude);
	Coefficients.Add(StatCoefficient);
}
