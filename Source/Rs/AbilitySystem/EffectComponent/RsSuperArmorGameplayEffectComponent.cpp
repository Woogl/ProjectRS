// Copyright 2024 Team BH.


#include "RsSuperArmorGameplayEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "Misc/DataValidation.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Effect/RsDamageDefinition.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"

#define LOCTEXT_NAMESPACE "RsSuperArmorGameplayEffectComponent"

URsSuperArmorGameplayEffectComponent::URsSuperArmorGameplayEffectComponent()
{
#if WITH_EDITORONLY_DATA
	EditorFriendlyName = TEXT("Immunity (Prevent hit reaction)");
#endif // WITH_EDITORONLY_DATA
}

bool URsSuperArmorGameplayEffectComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const
{
	FActiveGameplayEffectHandle& ActiveGEHandle = ActiveGE.Handle;
	UAbilitySystemComponent* OwnerASC = ActiveGEContainer.Owner;

	// Register our immunity query to potentially block applications of any Gameplay Effects
	FGameplayEffectApplicationQuery& BoundQuery = OwnerASC->GameplayEffectApplicationQueries.AddDefaulted_GetRef();
	BoundQuery.BindUObject(this, &ThisClass::AllowGameplayEffectApplication, ActiveGEHandle);

	// Now that we've bound that function, let's unbind it when we're removed.  This is safe because once we're removed, EventSet is gone.
	ActiveGE.EventSet.OnEffectRemoved.AddLambda([OwnerASC, QueryToRemove = BoundQuery.GetHandle()](const FGameplayEffectRemovalInfo& RemovalInfo)
		{
			if (ensure(IsValid(OwnerASC)))
			{
				TArray<FGameplayEffectApplicationQuery>& GEAppQueries = OwnerASC->GameplayEffectApplicationQueries;
				for (auto It = GEAppQueries.CreateIterator(); It; ++It)
				{
					if (It->GetHandle() == QueryToRemove)
					{
						It.RemoveCurrentSwap();
						break;
					}
				}
			}
		});
	return true;
}

bool URsSuperArmorGameplayEffectComponent::AllowGameplayEffectApplication(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpecToConsider, FActiveGameplayEffectHandle ImmunityActiveGE) const
{
	// SCOPE_CYCLE_COUNTER(STAT_HasApplicationImmunityToSpec)
	
	const UAbilitySystemComponent* ASC = ActiveGEContainer.Owner;
	if (ASC != ImmunityActiveGE.GetOwningAbilitySystemComponent())
	{
		ensureMsgf(false, TEXT("Something went wrong where an ActiveGameplayEffect jumped AbilitySystemComponents"));
		return false;
	}

	// ActiveGE can be null (IsPendingRemove) but we could have not yet received OnEffectRemoved. This can happen when applying more effects during its removal.
	const FActiveGameplayEffect* ActiveGE = ASC->GetActiveGameplayEffect(ImmunityActiveGE);
	if (!ActiveGE || ActiveGE->bIsInhibited)
	{
		return true;
	}
	
	FName SuperArmorPierceName = GET_MEMBER_NAME_CHECKED(URsDamageDefinition, SuperArmorPierce);
	float SuperArmorPierceTier = GESpecToConsider.GetSetByCallerMagnitude(SuperArmorPierceName, false, -1);
	float SuperArmorTier = ASC->GetNumericAttribute(URsDefenseSet::GetSuperArmorTierAttribute());

	// Pierce penetrates same-tier immunity.
	if ((SuperArmorPierceTier != -1) && (SuperArmorTier > SuperArmorPierceTier))
	{
		ASC->OnImmunityBlockGameplayEffectDelegate.Broadcast(GESpecToConsider, ActiveGE);
		return false;
	}

	return true;
}

#if WITH_EDITOR
EDataValidationResult URsSuperArmorGameplayEffectComponent::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (GetOwner()->DurationPolicy == EGameplayEffectDurationType::Instant)
	{
		Context.AddError(FText::FormatOrdered(LOCTEXT("IsInstantEffectError", "GE is an Instant Effect and incompatible with {0}"), FText::FromString(GetClass()->GetName())));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}
#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE