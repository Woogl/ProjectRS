// Copyright 2025 Team BH.


#include "RsGameplayEffect.h"

#include "GameplayEffectComponent.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/EffectComponent/RsModifierDataEffectComponent.h"
#include "UObject/ObjectSaveContext.h"

URsGameplayEffect::URsGameplayEffect()
{
	FGameplayEffectModifierMagnitude DefaultDuration(0);
	DurationMagnitude = DefaultDuration;
}

#if WITH_EDITOR
void URsGameplayEffect::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// Find modifier data.
	URsModifierDataEffectComponent* ModifierDataEffectComp = nullptr;
	for (TObjectPtr<UGameplayEffectComponent> GEComponent : GEComponents)
	{
		if (!GEComponent)
		{
			continue;
		}
		ModifierDataEffectComp = Cast<URsModifierDataEffectComponent>(GEComponent);
		if (ModifierDataEffectComp)
		{
			break;
		}
	}
	if (!ModifierDataEffectComp)
	{
		return;
	}

	Modifiers.Empty();

	if (ModifierDataEffectComp->DataTableRow.IsNull())
	{
		SetModifiersFromAsset(ModifierDataEffectComp);
	}
	else
	{
		SetModifiersFromTable(ModifierDataEffectComp);
	}
}

void URsGameplayEffect::SetModifiersFromAsset(const URsModifierDataEffectComponent* ModifierDataEffectComp)
{
	for (const FModifierCoefficient& ModCoeff : ModifierDataEffectComp->ModifierCoefficients)
	{
		// Stat to modify
		FGameplayAttribute Attribute = URsAbilitySystemLibrary::GetAttributeByTag(ModCoeff.Stat);

		for (const auto [CoeffTag, Coeff] : ModCoeff.Coefficients)
		{
			// (Coefficient.ATK.source * 1.5) + 500
			//                                   ^
			if (CoeffTag == RsGameplayTags::COEFFICIENT_MANUAL)
			{
				FGameplayModifierInfo ModifierInfo(Attribute, EGameplayModOp::AddFinal, FGameplayEffectModifierMagnitude(Coeff));
				Modifiers.Add(ModifierInfo);
				continue;
			}

			// (Coefficient.ATK.source * 1.5) + 500
			//                    ^
			FGameplayAttribute AttributeToCapture = URsAbilitySystemLibrary::GetAttributeByCoefficientTag(CoeffTag);
			EGameplayEffectAttributeCaptureSource SourceOrTarget;
			if (CoeffTag.ToString().EndsWith(TEXT(".Source")))
			{
				SourceOrTarget = EGameplayEffectAttributeCaptureSource::Source;
			}
			else if (CoeffTag.ToString().EndsWith(TEXT(".Target")))
			{
				SourceOrTarget = EGameplayEffectAttributeCaptureSource::Target;
			}
			else
			{
				UE_LOG(RsLog, Warning, TEXT("Coefficient tag { %s } must end with \".Target\" or \".Source\"."), *CoeffTag.ToString());
				continue;
			}
			bool bSnapshot = (SourceOrTarget == EGameplayEffectAttributeCaptureSource::Source);
			FGameplayEffectAttributeCaptureDefinition CaptureDefinition(AttributeToCapture, SourceOrTarget, bSnapshot);

			FAttributeBasedFloat MagnitudeData;
			// (Coefficient.ATK.source * 1.5) + 500
			//                            ^
			MagnitudeData.Coefficient = Coeff;
			// (Coefficient.ATK.source * 1.5) + 500
			//               ^
			MagnitudeData.BackingAttribute = CaptureDefinition;

			FGameplayModifierInfo ModifierInfo(Attribute, EGameplayModOp::Additive, MagnitudeData);
			Modifiers.Add(ModifierInfo);
		}
	}
}

void URsGameplayEffect::SetModifiersFromTable(const URsModifierDataEffectComponent* ModifierDataEffectComp)
{
	// TODO: Implement
}
#endif // WITH_EDITOR
