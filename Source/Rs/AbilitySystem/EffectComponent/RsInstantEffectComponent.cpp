// Copyright 2025 Team BH.


#include "RsInstantEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficient.h"
#include "Rs/AbilitySystem/Effect/RsUnitEffect.h"

void URsInstantEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);

	if (!ensureMsgf(ActiveGEContainer.Owner, TEXT("OnGameplayEffectApplied is passed an ActiveGEContainer which lives within an ASC but that ASC was somehow null")))
	{
		return;
	}
	
	FRsEffectCoefficient EffectCoefficient(Effect, Coefficients);
	UAbilitySystemComponent* SourceASC = GESpec.GetContext().GetInstigatorAbilitySystemComponent();

	FGameplayEffectSpecHandle EffectHandle = URsAbilitySystemLibrary::MakeEffectSpecCoefficient(SourceASC, EffectCoefficient, SourceASC->MakeEffectContext());
	// EffectHandle.Data->SetDuration(GESpec.Duration, true);

	URsAbilitySystemLibrary::ApplyEffectSpecCoefficient(SourceASC, ActiveGEContainer.Owner, EffectHandle);
}
