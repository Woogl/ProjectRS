// Copyright 2024 Team BH.


#include "RsAdditionalDamageEffectGameplayEffectComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Misc/DataValidation.h"

#define LOCTEXT_NAMESPACE "URsAdditionalDamageEffectGameplayEffectComponent"


void URsAdditionalDamageEffectGameplayEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);

	FGameplayTagContainer AdditionalEffectTagContainer = GESpec.GetDynamicAssetTags();
	if (!AdditionalEffectTagContainer.IsEmpty())
	{
		for (FGameplayTag AdditionalEffectTag : AdditionalEffectTagContainer)
		{
			if (TSubclassOf<UGameplayEffect> AdditionalEffectClass = AdditionalDamageEffectTable.FindRef(AdditionalEffectTag))
			{
				if (UGameplayEffect* EffectCDO = AdditionalEffectClass.GetDefaultObject())
				{
					FGameplayEffectSpec NewSpec;
					NewSpec.InitializeFromLinkedSpec(EffectCDO, GESpec);
					ActiveGEContainer.Owner->ApplyGameplayEffectSpecToSelf(NewSpec);
				}
			}
		}
	}
}

#if WITH_EDITOR
EDataValidationResult URsAdditionalDamageEffectGameplayEffectComponent::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	
	if (AdditionalDamageEffectTable.IsEmpty())
	{
		Result = EDataValidationResult::Invalid;
		Context.AddWarning(LOCTEXT("EmptyAdditionalEffectTable", "No Elements In Additional Damage Effect Table"));
	}
	return Result;
}
#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE

