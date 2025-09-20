// Copyright 2024 Team BH.


#include "RsCoefficientCalculation.h"

#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemSettings.h"

URsCoefficientCalculation::URsCoefficientCalculation()
{
	// Capture every attribute of source and target
	// NOTE: Attributes to be captured must exist in the source and target!
	for (const TTuple<FGameplayTag, FGameplayAttribute>& TaggedAttribute : URsAbilitySystemSettings::Get().TaggedAttributes)
	{
		FName SourceKey = FName(FString(TaggedAttribute.Key.ToString() + TEXT(".Source")));
		CaptureAttribute(SourceKey, TaggedAttribute.Value, EGameplayEffectAttributeCaptureSource::Source, true);
		
		FName TargetKey = FName(FString(TaggedAttribute.Key.ToString() + TEXT(".Target")));
		CaptureAttribute(TargetKey, TaggedAttribute.Value, EGameplayEffectAttributeCaptureSource::Target, false);
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
	for (const TTuple<FName, float>& SetByCaller : Spec.SetByCallerNameMagnitudes)
	{
		FName AttributeTagName = ChopLastDot(SetByCaller.Key);
		FGameplayTag AttributeTag = FGameplayTag::RequestGameplayTag(AttributeTagName, false);
		if (URsAbilitySystemSettings::Get().TaggedAttributes.Contains(AttributeTag))
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
	FinalMagnitude += Spec.GetSetByCallerMagnitude(RsGameplayTags::MANUAL, false);
	
	return FinalMagnitude;
}

void URsCoefficientCalculation::CaptureAttribute(FName Key, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource SourceOrTarget, bool bSnapShot)
{
	FGameplayEffectAttributeCaptureDefinition Definition;
	Definition.AttributeToCapture = Attribute;
	Definition.AttributeSource = SourceOrTarget;
	Definition.bSnapshot = bSnapShot;
	RelevantAttributesToCapture.Add(Definition);
	CapturedAttributeDefinitions.Add(Key, Definition);
}

float URsCoefficientCalculation::FindAttributeMagnitude(FName Key, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters) const
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

FName URsCoefficientCalculation::ChopLastDot(const FName& InName) const
{
	FString String = InName.ToString();
	int32 DotIndex;
	String.FindLastChar('.', DotIndex);
	if (DotIndex == INDEX_NONE)
	{
		return InName;
	}
	return FName(String.LeftChop(String.Len() - DotIndex));
}
