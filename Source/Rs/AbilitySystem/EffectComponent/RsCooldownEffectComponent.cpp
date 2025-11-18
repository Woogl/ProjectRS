// Copyright 2025 Team BH.


#include "RsCooldownEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

void URsCooldownEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);

	FGameplayTag LocalCooldownTag;
	ECooldownModifingType LocalModifingType;
	float LocalAmount;
	if (const FRsEffectTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsEffectTableRow>(GESpec))
	{
		// TODO: Implement
		// LocalCooldownTag = FGameplayTag::RequestGameplayTag(FName(Row->Parameter1));
		// int64 EnumValue = StaticEnum<ECooldownModifingType>()->GetValueByNameString(Row->Parameter2, EGetByNameFlags::ErrorIfNotFound);
		// LocalModifingType = static_cast<ECooldownModifingType>(EnumValue);
		// LocalAmount = FCString::Atof(*Row->Parameter3);
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
