// Copyright 2025 Team BH.


#include "RsAdditionalTargetEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemSettings.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

struct FRsEffectTableRowBase;

void URsAdditionalTargetEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);

	// Check data table
	// Should we add shared effect table???
	FDataTableRowHandle RowHandle = URsAbilitySystemGlobals::GetSetByCallerTableRowHandle(GESpec);
	if (RowHandle.IsNull())
	{
		return;
	}
	FRsDamageTableRow* CurrentEffectRow = RowHandle.GetRow<FRsDamageTableRow>(ANSI_TO_TCHAR(__FUNCTION__));
	if (!CurrentEffectRow)
	{
		return;
	}
	FName AdditionalEffectName = FName(CurrentEffectRow->AdditionalTargetEffect);
	FRsEffectTableRowBase* AdditionalEffectRow = RowHandle.DataTable->FindRow<FRsEffectTableRowBase>(AdditionalEffectName, ANSI_TO_TCHAR(__FUNCTION__));
	if (!AdditionalEffectRow)
	{
		return;
	}
	if (const TSubclassOf<UGameplayEffect> AdditionalEffect = AdditionalEffectRow->EffectClass)
	{
		if (UAbilitySystemComponent* TargetASC = ActiveGEContainer.Owner)
		{
			FGameplayEffectContextHandle AdditionalEffectContext = TargetASC->MakeEffectContext();
			FGameplayEffectSpecHandle AdditionalGESpec = TargetASC->MakeOutgoingSpec(AdditionalEffect, GESpec.GetLevel(), AdditionalEffectContext);
			if (AdditionalGESpec.IsValid())
			{
				// Data table feedback.
				FDataTableRowHandle AdditionalTableRowHandle;
				AdditionalTableRowHandle.DataTable = RowHandle.DataTable;
				AdditionalTableRowHandle.RowName = AdditionalEffectName;
				URsAbilitySystemGlobals::SetSetByCallerTableRowHandle(*AdditionalGESpec.Data, &AdditionalTableRowHandle);

				TargetASC->ApplyGameplayEffectSpecToSelf(*AdditionalGESpec.Data, PredictionKey);
			}
		}
	}
}
