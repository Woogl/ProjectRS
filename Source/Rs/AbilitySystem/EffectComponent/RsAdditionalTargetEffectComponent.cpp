// Copyright 2025 Team BH.


#include "RsAdditionalTargetEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

struct FRsEffectTableRowBase;

void URsAdditionalTargetEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);

	UAbilitySystemComponent* TargetASC = ActiveGEContainer.Owner;
	if (!TargetASC)
	{
		return;
	}
	
	// Check data table
	const FRsEffectTableRow* CurrentEffectRow = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsEffectTableRow>(GESpec);
	if (!CurrentEffectRow)
	{
		return;
	}
	FName AdditionalEffectName = CurrentEffectRow->FindValue<FName>(TEXT("AdditionalTargetEffect"), false);
	if (!AdditionalEffectName.IsValid() || AdditionalEffectName.IsNone())
	{
		return;
	}
	if (URsAbilitySystemComponent* RsSourceASC = Cast<URsAbilitySystemComponent>(ActiveGEContainer.Owner))
	{
		FGameplayEffectSpecHandle GESpecHandle = RsSourceASC->MakeOutgoingSpecFromSharedTable(AdditionalEffectName, GESpec.GetLevel());
		RsSourceASC->BP_ApplyGameplayEffectSpecToTarget(GESpecHandle, TargetASC);
	}
}
