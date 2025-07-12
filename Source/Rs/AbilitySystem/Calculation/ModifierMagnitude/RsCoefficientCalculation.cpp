// Copyright 2024 Team BH.


#include "RsCoefficientCalculation.h"

#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/System/RsDeveloperSetting.h"

URsCoefficientCalculation::URsCoefficientCalculation()
{
	// Capture every attribute of source and target
	// NOTE: Attributes to be captured must exist in the source and target!
	for (const TTuple<FGameplayTag, FGameplayAttribute>& CoefficientTag : URsDeveloperSetting::Get()->CoefficientTags)
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

void URsCoefficientCalculation::CaptureAttribute(FGameplayTag Key, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource SourceOrTarget, bool bSnapShot)
{
	FGameplayEffectAttributeCaptureDefinition Definition;
	Definition.AttributeToCapture = Attribute;
	Definition.AttributeSource = SourceOrTarget;
	Definition.bSnapshot = bSnapShot;
	RelevantAttributesToCapture.Add(Definition);
	CapturedAttributeDefinitions.Add(Key, Definition);
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
		if (URsDeveloperSetting::Get()->CoefficientTags.Contains(SetByCaller.Key))
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
	FinalMagnitude += Spec.GetSetByCallerMagnitude(RsGameplayTags::MANUAL_MAGNITUDE, false);
	
	return FinalMagnitude;
}
