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
	const URsModifierDataEffectComponent* ModifierDataEffectComp = FindModifierDataComponent();
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

bool URsGameplayEffect::CanEditChange(const FEditPropertyChain& PropertyChain) const
{
	bool bParentVal = Super::CanEditChange(PropertyChain);
	
	if (FindModifierDataComponent() != nullptr)
	{
		if (PropertyChain.GetHead()->GetValue()->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, Modifiers))
		{
			return false;
		}
	}
	return bParentVal;
}

const URsModifierDataEffectComponent* URsGameplayEffect::FindModifierDataComponent() const
{
	for (TObjectPtr<UGameplayEffectComponent> GEComponent : GEComponents)
	{
		if (!GEComponent)
		{
			continue;
		}
		if (URsModifierDataEffectComponent* ModifierDataComp = Cast<URsModifierDataEffectComponent>(GEComponent))
		{
			return ModifierDataComp;
		}
	}
	return nullptr;
}

void URsGameplayEffect::SetModifiersFromAsset(const URsModifierDataEffectComponent* ModifierDataComp)
{
	for (const FModifierCoefficient& ModCoeff : ModifierDataComp->ModifierCoefficients)
	{
		FGameplayAttribute StatToModify = URsAbilitySystemLibrary::GetAttributeByTag(ModCoeff.Stat);

		for (const auto [CoeffTag, CoeffNum] : ModCoeff.Coefficients)
		{
			// (Coefficient.ATK.source * 1.5) + 500
			//                                   ^
			if (CoeffTag == RsGameplayTags::COEFFICIENT_MANUAL)
			{
				FGameplayModifierInfo ModifierInfo(StatToModify, EGameplayModOp::AddFinal, FGameplayEffectModifierMagnitude(CoeffNum));
				Modifiers.Add(ModifierInfo);
				continue;
			}
			
			// (Coefficient.ATK.source * 1.5) + 500
			//               ^
			FGameplayAttribute StatToCapture = URsAbilitySystemLibrary::GetAttributeByCoefficientTag(CoeffTag);
			// (Coefficient.ATK.source * 1.5) + 500
			//                    ^
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
				UE_LOG(RsLog, Error, TEXT("Coefficient tag { %s } must end with \".Target\" or \".Source\"."), *CoeffTag.ToString());
				continue;
			}
			bool bSnapshot = (SourceOrTarget == EGameplayEffectAttributeCaptureSource::Source);
			FGameplayEffectAttributeCaptureDefinition CaptureDefinition(StatToCapture, SourceOrTarget, bSnapshot);

			FAttributeBasedFloat MagnitudeData;
			// (Coefficient.ATK.source * 1.5) + 500
			//                            ^
			MagnitudeData.Coefficient = CoeffNum;
			MagnitudeData.BackingAttribute = CaptureDefinition;

			FGameplayModifierInfo ModifierInfo(StatToModify, EGameplayModOp::Additive, MagnitudeData);
			Modifiers.Add(ModifierInfo);
		}
	}
}

void URsGameplayEffect::SetModifiersFromTable(const URsModifierDataEffectComponent* ModifierDataComp)
{
	// TODO: Implement
}
#endif // WITH_EDITOR
