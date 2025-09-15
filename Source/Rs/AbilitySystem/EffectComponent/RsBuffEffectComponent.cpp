// Copyright 2025 Team BH.


#include "RsBuffEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/RsAbilitySystemSettings.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficient.h"

void URsBuffEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	if (!ensureMsgf(ActiveGEContainer.Owner, TEXT("OnGameplayEffectApplied is passed an ActiveGEContainer which lives within an ASC but that ASC was somehow null")))
	{
		return;
	}

	UAbilitySystemComponent* AppliedToASC = nullptr;
	if (TargetType == ERsEffectTarget::Source)
	{
		AppliedToASC = GESpec.GetContext().GetInstigatorAbilitySystemComponent();
	}
	else if (TargetType == ERsEffectTarget::Target)
	{
		AppliedToASC = ActiveGEContainer.Owner;
	}

	if (AppliedToASC)
	{
		if (TSubclassOf<UGameplayEffect> BuffEffect = URsAbilitySystemSettings::Get().BuffEffects.FindRef(StatTag))
		{
			FRsEffectCoefficient EffectCoefficient(BuffEffect, Coefficients, GESpec.GetDuration());
			AActor* Source = GESpec.GetContext().GetInstigator();
			AActor* Target = AppliedToASC->GetOwnerActor();
			URsAbilitySystemLibrary::ApplyEffectCoefficient(Source, Target, EffectCoefficient);
		}
	}
}
