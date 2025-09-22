// Copyright 2024 Team BH.


#include "RsCoefficientCalculation.h"

#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemSettings.h"

URsCoefficientCalculation::URsCoefficientCalculation()
{
	// Capture every attribute of source and target
	// NOTE: Attributes to be captured must exist in the source and target!
    for (const TTuple<FGameplayTag, FGameplayAttribute>& CoefficientTag : URsAbilitySystemSettings::Get().Coefficients)
    {
    	if (CoefficientTag.Key.ToString().EndsWith(TEXT("Source")))
    	{
    		CaptureAttribute(CoefficientTag.Key, CoefficientTag.Value, EGameplayEffectAttributeCaptureSource::Source, true);
    	}
    	else if (CoefficientTag.Key.ToString().EndsWith(TEXT("Target")))
    	{
    		CaptureAttribute(CoefficientTag.Key, CoefficientTag.Value, EGameplayEffectAttributeCaptureSource::Target, false);
    	}
	}
}

float URsCoefficientCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	// Accumulate every "Coefficient * Attribute"
	float FinalMagnitude = 0.f;
	for (const TTuple<FGameplayTag, float>& SetByCaller : Spec.SetByCallerTagMagnitudes)
	{
		FGameplayAttribute FoundAttribute = URsAbilitySystemSettings::Get().FindAttributeFromCoefficientTag(SetByCaller.Key);
		if (FoundAttribute.IsValid())
		{
			float Coefficient = SetByCaller.Value;
			if (FMath::IsNearlyZero(Coefficient) == false)
			{
				float Attribute = FindAttributeMagnitude(SetByCaller.Key, Spec, EvaluationParameters);
				FinalMagnitude += Coefficient * Attribute;
			}
		}
	}

	// Add "Manual" magnitude
	FinalMagnitude += Spec.GetSetByCallerMagnitude(RsGameplayTags::COEFFICIENT_MANUAL, false);
	
	return FinalMagnitude;
}

float URsCoefficientCalculation::FindAttributeMagnitude(FGameplayTag Key, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters) const
{
	float OutMagnitude = 0.f;
	if (CapturedAttributeDefinitions.Contains(Key))
	{
		GetCapturedAttributeMagnitude(CapturedAttributeDefinitions[Key], Spec, EvaluationParameters, OutMagnitude);
	}
	else
	{
		UE_LOG(RsLog, Warning, TEXT("Cannot find {%s} attribute"), *Key.ToString());
	}
	return OutMagnitude;
}

void URsCoefficientCalculation::CaptureAttribute(FGameplayTag Key, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource SourceOrTarget, bool bSnapShot)
{
	FGameplayEffectAttributeCaptureDefinition Definition;
	Definition.AttributeToCapture = Attribute;
	Definition.AttributeSource = SourceOrTarget;
	Definition.bSnapshot = bSnapShot;
	RelevantAttributesToCapture.Add(Definition);
	CapturedAttributeDefinitions.Add(Key, Definition);
}
