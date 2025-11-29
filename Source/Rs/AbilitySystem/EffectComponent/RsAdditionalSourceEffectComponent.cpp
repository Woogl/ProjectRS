// Copyright 2025 Team BH.


#include "RsAdditionalSourceEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "GameplayEffect.h"
#include "Misc/DataValidation.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

#define LOCTEXT_NAMESPACE "RsAdditionalSourceEffectComponent"

bool URsAdditionalSourceEffectComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const
{
	// We don't allow prediction of expiration (on removed) effects
	if (ActiveGEContainer.IsNetAuthority())
	{
		// When this ActiveGE gets removed, so will our events so no need to unbind this.
		ActiveGE.EventSet.OnEffectRemoved.AddUObject(this, &ThisClass::OnActiveGameplayEffectRemoved, &ActiveGEContainer);
	}

	return true;
}

void URsAdditionalSourceEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	if (!ensureMsgf(ActiveGEContainer.Owner, TEXT("OnGameplayEffectApplied is passed an ActiveGEContainer which lives within an ASC but that ASC was somehow null")))
	{
		return;
	}

	const float GELevel = GESpec.GetLevel();
	const FGameplayEffectContextHandle& GEContextHandle = GESpec.GetEffectContext();

	/** other effects that need to be applied to the source if this effect is successful */
	TArray<FGameplayEffectSpecHandle> SourceEffectSpecs;
	for (const TSubclassOf<UGameplayEffect>& EffectClass : OnApplicationGameplayEffects)
	{
		const UGameplayEffect* GameplayEffectDef = EffectClass.GetDefaultObject();
		if (!GameplayEffectDef)
		{
			continue;
		}

		FGameplayEffectSpecHandle SpecHandle = GESpec.GetEffectContext().GetInstigatorAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, GELevel, GEContextHandle);
		if (SpecHandle.IsValid())
		{
			SourceEffectSpecs.Add(SpecHandle);
		}
	}

	// Add all of the dynamically linked ones that don't have any conditions applied to them
	// Note: I believe this functionality should be removed as its usage is dynamic and thus not 
	// synchronized between client/server.  I've deprecated the variable to help enforce this.
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	SourceEffectSpecs.Append(GESpec.TargetEffectSpecs);
PRAGMA_ENABLE_DEPRECATION_WARNINGS

	// Apply to source!!!
	UAbilitySystemComponent& AppliedToASC = *GESpec.GetContext().GetInstigatorAbilitySystemComponent();
	for (const FGameplayEffectSpecHandle& SourceSpec : SourceEffectSpecs)
	{
		if (SourceSpec.IsValid())
		{
			AppliedToASC.ApplyGameplayEffectSpecToSelf(*SourceSpec.Data.Get(), PredictionKey);
		}
	}

	URsAbilitySystemComponent* SourceASC = Cast<URsAbilitySystemComponent>(&AppliedToASC);
	URsAbilitySystemComponent* TargetASC = Cast<URsAbilitySystemComponent>(ActiveGEContainer.Owner);
	if (!SourceASC || !TargetASC)
	{
		return;
	}

	// Check data table
	const FRsEffectTableRow* CurrentEffectRow = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsEffectTableRow>(GESpec);
	if (!CurrentEffectRow)
	{
		return;
	}
	FName AdditionalEffectName = CurrentEffectRow->FindValue<FName>(TEXT("AdditionalSourceEffect"), false);
	if (!AdditionalEffectName.IsValid() || AdditionalEffectName.IsNone())
	{
		return;
	}
	if (const UGameplayAbility* Ability = GESpec.GetContext().GetAbility())
	{
		if (const URsGameplayAbility* RsAbility = Cast<URsGameplayAbility>(Ability))
		{
			FGameplayEffectSpecHandle AdditionalEffectSpec = RsAbility->MakeOutgoingTableEffect(AdditionalEffectName, TargetASC, TargetASC->MakeEffectContext());
			TargetASC->BP_ApplyGameplayEffectSpecToTarget(AdditionalEffectSpec, SourceASC);
		}
	}
}

void URsAdditionalSourceEffectComponent::OnActiveGameplayEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo, FActiveGameplayEffectsContainer* ActiveGEContainer) const
{
	FScopedActiveGameplayEffectLock ActiveScopeLock(*ActiveGEContainer);
	
	const FActiveGameplayEffect* ActiveGE = RemovalInfo.ActiveEffect;
	if (!ensure(ActiveGE))
	{
		UE_LOG(LogGameplayEffects, Error, TEXT("FGameplayEffectRemovalInfo::ActiveEffect was not populated in OnActiveGameplayEffectRemoved"));
		return;
	}

	// Apply to source!!!
	UAbilitySystemComponent* ASC = ActiveGEContainer->Owner;
	if (!ASC)
	{
		UE_LOG(LogGameplayEffects, Verbose, TEXT("InstigatorAbilitySystemComponent was invalid in OnActiveGameplayEffectRemoved"));
		return;
	}

	// Determine the appropriate type of effect to apply depending on whether the effect is being prematurely removed or not
	const TArray<TSubclassOf<UGameplayEffect>>& ExpiryEffects = (RemovalInfo.bPrematureRemoval ? OnCompletePrematurely : OnCompleteNormal);

	// Mix-in the always-executing GameplayEffects
	TArray<TSubclassOf<UGameplayEffect>> AllGameplayEffects{ ExpiryEffects };
	AllGameplayEffects.Append(OnCompleteAlways);

	for (TSubclassOf<UGameplayEffect>& CurExpiryEffect : AllGameplayEffects)
	{
		if (UGameplayEffect* CurExpiryCDO = CurExpiryEffect.GetDefaultObject())
		{
			FGameplayEffectSpec NewSpec;
			NewSpec.InitializeFromLinkedSpec(CurExpiryCDO, ActiveGE->Spec);

			ASC->ApplyGameplayEffectSpecToSelf(NewSpec);
		}
	}
}

#if WITH_EDITOR
EDataValidationResult URsAdditionalSourceEffectComponent::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (GetOwner()->DurationPolicy == EGameplayEffectDurationType::Instant)
	{
		const bool bHasOnCompleteEffects = (OnCompleteAlways.Num() + OnCompleteNormal.Num() + OnCompletePrematurely.Num() > 0);
		if (bHasOnCompleteEffects)
		{
			Context.AddError(FText::FormatOrdered(LOCTEXT("InstantDoesNotWorkWithOnComplete", "Instant GE will never receive OnComplete for {0}."), FText::FromString(GetClass()->GetName())));
			Result = EDataValidationResult::Invalid;
		}
	}
	else if (GetOwner()->Period.Value > 0.0f)
	{
		if (OnApplicationGameplayEffects.Num() > 0)
		{
			Context.AddWarning(LOCTEXT("IsPeriodicAndHasOnApplication", "Periodic GE has OnApplicationGameplayEffects. Those GE's will only be applied once."));
		}
	}

	return Result;
}
#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE
