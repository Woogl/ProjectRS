// Copyright 2025 Team BH.


#include "RsBuffEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficient.h"
#include "Rs/Battle/RsBattleSettings.h"

void URsBuffEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	if (!ensureMsgf(ActiveGEContainer.Owner, TEXT("OnGameplayEffectApplied is passed an ActiveGEContainer which lives within an ASC but that ASC was somehow null")))
	{
		return;
	}

	UAbilitySystemComponent* AppliedToASC = nullptr;
	if (Target == ERsEffectTarget::Source)
	{
		AppliedToASC = GESpec.GetEffectContext().GetInstigatorAbilitySystemComponent();
	}
	else if (Target == ERsEffectTarget::Target)
	{
		AppliedToASC = ActiveGEContainer.Owner;
	}

	if (AppliedToASC)
	{
		TSubclassOf<UGameplayEffect> BuffEffect = URsBattleSettings::Get().BuffEffects.FindRef(Stat);
		FRsEffectCoefficient EffectCoefficient(BuffEffect, Coefficients);
		FGameplayEffectSpecHandle BuffSpec = URsAbilitySystemLibrary::MakeEffectSpecCoefficient(AppliedToASC, EffectCoefficient, AppliedToASC->MakeEffectContext());
		AppliedToASC->ApplyGameplayEffectSpecToSelf(*BuffSpec.Data.Get(), PredictionKey);
	}
}
