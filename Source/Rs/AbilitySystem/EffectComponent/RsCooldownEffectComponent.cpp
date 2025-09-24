// Copyright 2025 Team BH.


#include "RsCooldownEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

void URsCooldownEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);

	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
	UAbilitySystemComponent* TargetASC = ActiveGEContainer.Owner;
	TArray<FActiveGameplayEffectHandle> CooldownEffects = TargetASC->GetActiveEffects(Query);
	for (const FActiveGameplayEffectHandle& EffectHandle : CooldownEffects)
	{
		if (const FActiveGameplayEffect* CooldownEffect = TargetASC->GetActiveGameplayEffect(EffectHandle))
		{
			if (ModifingType == ECooldownModifingType::Add)
			{
				TargetASC->ModifyActiveEffectStartTime(EffectHandle, Amount);
			}
			else if (ModifingType == ECooldownModifingType::Set)
			{
				float TimeRemaining = CooldownEffect->GetTimeRemaining(GetWorld()->GetTimeSeconds());
				TargetASC->ModifyActiveEffectStartTime(EffectHandle, -TimeRemaining + Amount);
			}
		}
	}
}
