// Copyright 2025 Team BH.


#include "RsGameplayEffect.h"

#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/RsAbilitySystemSettings.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"
#include "Rs/AbilitySystem/Calculation/Execution/RsDamageResponseExecution.h"
#include "Rs/AbilitySystem/Calculation/Execution/RsHealthDamageExecution.h"
#include "Rs/AbilitySystem/Calculation/Execution/RsStaggerDamageExecution.h"
#include "Rs/AbilitySystem/EffectComponent/RsCanApplyEffectComponent.h"
#include "Rs/AbilitySystem/EffectComponent/RsDamageEffectComponent.h"
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
	
	const URsModifierDataEffectComponent* ModifierDataComp = FindComponent<URsModifierDataEffectComponent>();
	const URsDamageEffectComponent* DamageDataComp = FindComponent<URsDamageEffectComponent>();
	if (ModifierDataComp || DamageDataComp)
	{
		Modifiers.Empty();
	}

	if (ModifierDataComp)
	{
		if (ModifierDataComp->DataTableRow.IsNull())
		{
			SetModifiersFromAsset(ModifierDataComp);
		}
		else
		{
			SetModifiersFromTable(ModifierDataComp);
		}
	}

	if (DamageDataComp)
	{
		if (DamageDataComp->DataTableRow.IsNull())
		{
			SetModifiersFromAsset(DamageDataComp);
		}
		else
		{
			SetModifiersFromTable(DamageDataComp);
		}
		//Executions.Empty();
		SetDamageExecutions(DamageDataComp);
		//SetEffectComponents(DamageDataComp);
	}
}

bool URsGameplayEffect::CanEditChange(const FEditPropertyChain& PropertyChain) const
{
	bool bParentVal = Super::CanEditChange(PropertyChain);

	FName PropertyName = PropertyChain.GetHead()->GetValue()->GetFName();
	
	if (FindComponent<URsModifierDataEffectComponent>() != nullptr)
	{
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, Modifiers))
		{
			return false;
		}
	}

	if (FindComponent<URsDamageEffectComponent>() != nullptr)
	{
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, Modifiers))
		{
			return false;
		}
	}
	
	return bParentVal;
}

void URsGameplayEffect::AddModifier(const FGameplayModifierInfo& Modifier)
{
	Modifiers.Add(Modifier);
}

void URsGameplayEffect::AddExecution(const FGameplayEffectExecutionDefinition& Execution)
{
	Executions.Add(Execution);
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

void URsGameplayEffect::SetModifiersFromAsset(const URsDamageEffectComponent* DamageDataComp)
{
	// Health base damage
	for (const auto [CoeffTag, CoeffNum] : DamageDataComp->HealthDamageCoefficients)
	{
		// (Coefficient.ATK.source * 1.5) + 500
		//                                   ^
		if (CoeffTag == RsGameplayTags::COEFFICIENT_MANUAL)
		{
			FGameplayModifierInfo ModifierInfo(URsHealthSet::GetBaseDamageAttribute(), EGameplayModOp::AddFinal, FGameplayEffectModifierMagnitude(CoeffNum));
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

		FGameplayModifierInfo ModifierInfo(URsHealthSet::GetBaseDamageAttribute(), EGameplayModOp::Additive, MagnitudeData);
		Modifiers.Add(ModifierInfo);
	}

	// Stagger base damage
	for (const auto [CoeffTag, CoeffNum] : DamageDataComp->StaggerDamageCoefficients)
	{
		// (Coefficient.IMP.source * 1.5) + 500
		//                                   ^
		if (CoeffTag == RsGameplayTags::COEFFICIENT_MANUAL)
		{
			FGameplayModifierInfo ModifierInfo(URsStaggerSet::GetBaseDamageAttribute(), EGameplayModOp::AddFinal, FGameplayEffectModifierMagnitude(CoeffNum));
			Modifiers.Add(ModifierInfo);
			continue;
		}
			
		// (Coefficient.IMP.source * 1.5) + 500
		//               ^
		FGameplayAttribute StatToCapture = URsAbilitySystemLibrary::GetAttributeByCoefficientTag(CoeffTag);
		// (Coefficient.IMP.source * 1.5) + 500
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
		// (Coefficient.IMP.source * 1.5) + 500
		//                            ^
		MagnitudeData.Coefficient = CoeffNum;
		MagnitudeData.BackingAttribute = CaptureDefinition;

		FGameplayModifierInfo ModifierInfo(URsStaggerSet::GetBaseDamageAttribute(), EGameplayModOp::Additive, MagnitudeData);
		Modifiers.Add(ModifierInfo);
	}
}

void URsGameplayEffect::SetModifiersFromTable(const URsDamageEffectComponent* DamageDataComp)
{
	// TODO: Implement
}

void URsGameplayEffect::SetDamageExecutions(const URsDamageEffectComponent* DamageDataComp)
{
	bool bHasHealthDamageExecution = false;
	bool bHasStaggerDamageExecution = false;
	bool bHasDamageResponseExecution = false;
	for (const FGameplayEffectExecutionDefinition& Execution : Executions)
	{
		if (Execution.CalculationClass == URsHealthDamageExecution::StaticClass())
		{
			bHasHealthDamageExecution = true;
		}
		else if (Execution.CalculationClass == URsStaggerDamageExecution::StaticClass())
		{
			bHasStaggerDamageExecution = true;
		}
		else if (Execution.CalculationClass == URsDamageResponseExecution::StaticClass())
		{
			bHasDamageResponseExecution = true;
		}
	}

	if (!bHasHealthDamageExecution)
	{
		FGameplayEffectExecutionDefinition HealthDamageExecution(URsHealthDamageExecution::StaticClass());
		Executions.Add(HealthDamageExecution);
	}
	if (!bHasStaggerDamageExecution)
	{
		FGameplayEffectExecutionDefinition StaggerDamageExecution(URsStaggerDamageExecution::StaticClass());
		Executions.Add(StaggerDamageExecution);
	}
	if (!bHasDamageResponseExecution)
	{
		FGameplayEffectExecutionDefinition DamageResponseExecution(URsDamageResponseExecution::StaticClass());
		Executions.Add(DamageResponseExecution);
	}
}

// void URsGameplayEffect::SetEffectComponents(const URsDamageDataEffectComponent* DamageDataComp)
// {
// 	const UAssetTagsGameplayEffectComponent* DamageTagsComp = FindComponent<UAssetTagsGameplayEffectComponent>();
// 	if (!DamageTagsComp)
// 	{
// 		UAssetTagsGameplayEffectComponent* NewComp = NewObject<UAssetTagsGameplayEffectComponent>(this, TEXT("DamageTags"), GetMaskedFlags(RF_PropagateToSubObjects) | RF_Transactional);
// 		FInheritedTagContainer AssetTags;
// 		for (FGameplayTag DamageTag : DamageDataComp->DamageTags)
// 		{
// 			AssetTags.AddTag(DamageTag);
// 		}
// 		NewComp->SetAndApplyAssetTagChanges(AssetTags);
// 		GEComponents.Add(NewComp);
// 	}
//
// 	const URsCanApplyEffectComponent* CanApplyComp = FindComponent<URsCanApplyEffectComponent>();
// 	if (!CanApplyComp)
// 	{
// 		URsCanApplyEffectComponent* NewComp = NewObject<URsCanApplyEffectComponent>(this, TEXT("CanApply"), GetMaskedFlags(RF_PropagateToSubObjects) | RF_Transactional);
// 		NewComp->Stat = RsGameplayTags::STAT_INV;
// 		NewComp->Comparision = ERsComparisionOperator::Less;
// 		NewComp->Value = DamageDataComp->InvinciblePierce + 1;
// 		GEComponents.Add(NewComp);
// 	}
//
// 	const URsCrowdControlEffectComponent* CrowdControlComp = FindComponent<URsCrowdControlEffectComponent>();
// 	if (!CrowdControlComp)
// 	{
// 		URsCrowdControlEffectComponent* NewComp = NewObject<URsCrowdControlEffectComponent>(this, TEXT("CrowdControl"), GetMaskedFlags(RF_PropagateToSubObjects) | RF_Transactional);
// 		NewComp->EventTag = DamageDataComp->HitReaction;
// 		NewComp->bUseInternalCanApply = true;
// 		NewComp->Value = DamageDataComp->SuperArmorPierce + 1;
// 		GEComponents.Add(NewComp);
// 	}
//
// 	const URsHitStopEffectComponent* HitStopComp = FindComponent<URsHitStopEffectComponent>();
// 	if (!HitStopComp)
// 	{
// 		URsHitStopEffectComponent* NewComp = NewObject<URsHitStopEffectComponent>(this, TEXT("HitStop"), GetMaskedFlags(RF_PropagateToSubObjects) | RF_Transactional);
// 		NewComp->SourceHitStopTime = DamageDataComp->SourceHitStopTime;
// 		NewComp->TargetHitStopTime = DamageDataComp->TargetHitStopTime;
// 		GEComponents.Add(NewComp);
// 	}
// }
#endif // WITH_EDITOR
