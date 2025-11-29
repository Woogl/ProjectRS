// Copyright 2025 Team BH.


#include "RsAdditionalTargetEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

struct FRsEffectTableRowBase;

void URsAdditionalTargetEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);

	UAbilitySystemComponent* SourceASC = GESpec.GetContext().GetInstigatorAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ActiveGEContainer.Owner;
	
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
	if (const UGameplayAbility* Ability = GESpec.GetContext().GetAbility())
	{
		const URsGameplayAbility* RsAbility = Cast<URsGameplayAbility>(Ability);
		FGameplayEffectSpecHandle AdditionalEffectSpec = RsAbility->MakeOutgoingTableEffect(AdditionalEffectName, SourceASC, GESpec.GetContext());
		SourceASC->BP_ApplyGameplayEffectSpecToTarget(AdditionalEffectSpec, TargetASC);
	}
}
