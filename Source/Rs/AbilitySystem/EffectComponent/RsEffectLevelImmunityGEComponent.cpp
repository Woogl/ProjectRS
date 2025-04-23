// Copyright 2024 Team BH.


#include "RsEffectLevelImmunityGEComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectComponents/ImmunityGameplayEffectComponent.h"
#include "Misc/DataValidation.h"
#include "Rs/System/RsDeveloperSetting.h"

#define LOCTEXT_NAMESPACE "URsEffectLevelImmunityGEComponent"

bool URsEffectLevelImmunityGEComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const
{
	FActiveGameplayEffectHandle& ActiveGEHandle = ActiveGE.Handle;
	UAbilitySystemComponent* OwnerASC = ActiveGEContainer.Owner;

	// Register our immunity query to potentially block applications of any Gameplay Effects
	FGameplayEffectApplicationQuery& BoundQuery = OwnerASC->GameplayEffectApplicationQueries.AddDefaulted_GetRef();
	BoundQuery.BindUObject(this, &URsEffectLevelImmunityGEComponent::AllowGameplayEffectApplication, ActiveGEHandle);

	// if level is not set, consider it as level 1 GE.
	if (!ActiveGE.Spec.GetSetByCallerMagnitude(URsDeveloperSetting::Get()->ManualLevelTag))
	{
		ActiveGE.Spec.SetSetByCallerMagnitude(URsDeveloperSetting::Get()->ManualLevelTag, 1.f);
	}
	
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

bool URsEffectLevelImmunityGEComponent::AllowGameplayEffectApplication(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpecToConsider, FActiveGameplayEffectHandle ImmunityActiveGEHandle) const
{
	//SCOPE_CYCLE_COUNTER(STAT_HasApplicationImmunityToSpec)

	const UAbilitySystemComponent* ASC = ActiveGEContainer.Owner;
	if (ASC != ImmunityActiveGEHandle.GetOwningAbilitySystemComponent())
	{
		ensureMsgf(false, TEXT("Something went wrong where an ActiveGameplayEffect jumped AbilitySystemComponents"));
		return false;
	}

	// ActiveGE can be null (IsPendingRemove) but we could have not yet received OnEffectRemoved. This can happen when applying more effects during its removal.
	const FActiveGameplayEffect* ActiveGE = ASC->GetActiveGameplayEffect(ImmunityActiveGEHandle);
	if (!ActiveGE || ActiveGE->bIsInhibited)
	{
		return true;
	}

	for (const FGameplayEffectQuery& ImmunityQuery : ImmunityQueries)
	{
		if (!ImmunityQuery.IsEmpty() && ImmunityQuery.Matches(GESpecToConsider))
		{
			float EffectLevel = ActiveGEContainer.GetActiveGameplayEffect(ImmunityActiveGEHandle)->Spec.GetSetByCallerMagnitude(URsDeveloperSetting::Get()->ManualLevelTag);
			float ImmuneConsiderEffectLevel = GESpecToConsider.GetSetByCallerMagnitude(URsDeveloperSetting::Get()->ManualLevelTag);
			if (EffectLevel > ImmuneConsiderEffectLevel)
			{
				ASC->OnImmunityBlockGameplayEffectDelegate.Broadcast(GESpecToConsider, ActiveGE);
				return false;
			}
			return true;
		}
	}
	return true;
}

#if WITH_EDITOR
EDataValidationResult URsEffectLevelImmunityGEComponent::IsDataValid(FDataValidationContext& Context) const
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
