// Copyright 2024 Team BH.


#include "RsCoefficientCalculationBase.h"

#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"

URsCoefficientCalculationBase::URsCoefficientCalculationBase()
{
	// Capture every attribute of source and target
	// NOTE: Attributes to be captured must exist in the source and target!
	// TODO: CoefficientMap is not filled when construction!!!
	for (const auto [Tag, Coefficient] : URsAttributeSetBase::GetCoefficientMap())
    {
	    FString CoeffString = Tag.ToString();
	    if (CoeffString.EndsWith(TEXT(".Source")))
	    {
	    	CaptureAttribute(Tag, Coefficient, EGameplayEffectAttributeCaptureSource::Source, true);
	    }
    	else if (CoeffString.EndsWith(TEXT(".Target")))
    	{
    		CaptureAttribute(Tag, Coefficient, EGameplayEffectAttributeCaptureSource::Target, false);
    	}
	}
}

float URsCoefficientCalculationBase::FindAttributeMagnitude(FGameplayTag Key, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters) const
{
	float OutMagnitude = 0.f;
	if (CapturedAttributeDefinitions.Contains(Key))
	{
		GetCapturedAttributeMagnitude(CapturedAttributeDefinitions[Key], Spec, EvaluationParameters, OutMagnitude);
	}
	else
	{
		UE_LOG(RsLog, Warning, TEXT("Cannot find [%s] attribute"), *Key.ToString());
	}
	return OutMagnitude;
}

void URsCoefficientCalculationBase::CaptureAttribute(FGameplayTag Key, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource SourceOrTarget, bool bSnapShot)
{
	FGameplayEffectAttributeCaptureDefinition Definition;
	Definition.AttributeToCapture = Attribute;
	Definition.AttributeSource = SourceOrTarget;
	Definition.bSnapshot = bSnapShot;
	RelevantAttributesToCapture.Add(Definition);
	CapturedAttributeDefinitions.Add(Key, Definition);
}
