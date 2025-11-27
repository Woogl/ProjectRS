// Copyright 2025 Team BH.


#include "RsAbilityCooldownEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

void URsAbilityCooldownEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);

	FGameplayTag LocalCooldownTag;
	ECooldownModifingType LocalModifingType;
	float LocalAmount;
	if (const FRsEffectTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsEffectTableRow>(GESpec))
	{
		LocalCooldownTag = Row->FindValue<FGameplayTag>(TEXT("CooldownTag"), true);
		LocalModifingType = Row->FindValue<ECooldownModifingType>(TEXT("ModifyingType"), true);
		LocalAmount = Row->FindValue<float>(TEXT("Amount"), true);
	}
	else
	{
		LocalCooldownTag = CooldownTag;
		LocalModifingType = ModifingType;
		LocalAmount = Amount;
	}
	
	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(LocalCooldownTag.GetSingleTagContainer());
	UAbilitySystemComponent* TargetASC = ActiveGEContainer.Owner;
	TArray<FActiveGameplayEffectHandle> CooldownEffects = TargetASC->GetActiveEffects(Query);
	for (const FActiveGameplayEffectHandle& EffectHandle : CooldownEffects)
	{
		if (const FActiveGameplayEffect* CooldownEffect = TargetASC->GetActiveGameplayEffect(EffectHandle))
		{
			if (LocalModifingType == ECooldownModifingType::Add)
			{
				TargetASC->ModifyActiveEffectStartTime(EffectHandle, LocalAmount);
			}
			else if (LocalModifingType == ECooldownModifingType::Set)
			{
				float TimeRemaining = CooldownEffect->GetTimeRemaining(TargetASC->GetWorld()->GetTimeSeconds());
				TargetASC->ModifyActiveEffectStartTime(EffectHandle, -TimeRemaining + LocalAmount);
			}
		}
	}
}
