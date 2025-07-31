// Copyright 2024 Team BH.


#include "RsEffectDefinition.h"

#include "AbilitySystemComponent.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_PauseMontage.h"
#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/System/RsDeveloperSetting.h"
#include "Rs/Targeting/RsTargetingTypes.h"

void URsEffectDefinition_DamageBase::PostInitProperties()
{
	Super::PostInitProperties();

	// Cache a convenient pointer to the developer setting.
	DeveloperSetting = URsDeveloperSetting::Get();
}

FGameplayEffectContextHandle URsEffectDefinition_DamageBase::MakeDamageEffectContext(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) const
{
	if (!SourceASC || !TargetASC)
	{
		return FGameplayEffectContextHandle();
	}
	
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	
	AActor* SourceActor = SourceASC->GetAvatarActor();
	AActor* TargetActor = TargetASC->GetAvatarActor();
	FHitResult HitResult(TargetActor, nullptr, TargetActor->GetActorLocation(), FVector());
	if (!SourceActor || !TargetActor)
	{
		EffectContext.AddHitResult(HitResult);
		return EffectContext;
	}

	FVector Start = SourceActor->GetActorLocation();
	FVector End = TargetActor->GetActorLocation();
	// TODO: RsWeapon class
	if (USceneComponent* WeaponComponent = SourceActor->FindComponentByTag<USceneComponent>(TEXT("Weapon")))
	{
		Start = WeaponComponent->GetComponentLocation();
	}
	
	bool bHit = SourceActor->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (!bHit)
	{
		HitResult = FHitResult(TargetActor, nullptr, End, FVector());
	}

	// TODO: Set RsWeapon class to effect causer
	EffectContext.AddInstigator(SourceActor, nullptr);
	EffectContext.AddHitResult(HitResult);
	return EffectContext;
}

void URsEffectDefinition_DamageBase::ApplyInstantDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FRsEffectCoefficient& RsCoeff)
{
	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	FGameplayEffectSpecHandle DamageSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, RsCoeff, EffectContext);
	if (DamageSpec.IsValid())
	{
		SET_SETBYCALLER_PROPERTY(DamageSpec, InvinciblePierce);
		DamageSpec.Data->AppendDynamicAssetTags(DamageTags);
		SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpec.Data, TargetASC);
	}
}

void URsEffectDefinition_DamageBase::ApplyDotDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FRsEffectCoefficient& RsCoeff, float Duration, float Period)
{
	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	FGameplayEffectSpecHandle DotDamageSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, RsCoeff, EffectContext);
	if (DotDamageSpec.IsValid())
	{
		DotDamageSpec.Data->SetSetByCallerMagnitude(RsGameplayTags::MANUAL_DURATION, Duration);
		DotDamageSpec.Data->Period = Period;
		SET_SETBYCALLER_PROPERTY(DotDamageSpec, InvinciblePierce);
		DotDamageSpec.Data->AppendDynamicAssetTags(DamageTags);
		SourceASC->ApplyGameplayEffectSpecToTarget(*DotDamageSpec.Data, TargetASC);
	}
}

void URsEffectDefinition_DamageBase::ApplyHitReaction(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!HitReaction.IsValid())
	{
		return;
	}
	
	TSubclassOf<UGameplayEffect> HitReactionEffectClass = DeveloperSetting->HitReactionEffectClasses.FindRef(HitReaction);
	if (!HitReactionEffectClass)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	FGameplayEffectSpecHandle HitReactionSpec = SourceASC->MakeOutgoingSpec(HitReactionEffectClass, 0.f, EffectContext);
	if (HitReactionSpec.IsValid())
	{
		SET_SETBYCALLER_PROPERTY(HitReactionSpec, InvinciblePierce);
		SET_SETBYCALLER_PROPERTY(HitReactionSpec, SuperArmorPierce);
		SourceASC->ApplyGameplayEffectSpecToTarget(*HitReactionSpec.Data, TargetASC);
	}
}

URsEffectDefinition_InstantDamage::URsEffectDefinition_InstantDamage()
{
	// Set default coefficient.
	HealthDamageCoefficients.Add(RsGameplayTags::COEFFICIENT_ATTACK_SOURCE, 1.f);
	StaggerDamageCoefficients.Add(RsGameplayTags::COEFFICIENT_IMPACT_SOURCE, 1.f);
}

void URsEffectDefinition_InstantDamage::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	FRsEffectCoefficient RsHealthDamageCoeff(DeveloperSetting->HealthDamageEffectClass, HealthDamageCoefficients);
	ApplyInstantDamage(SourceASC, TargetASC, RsHealthDamageCoeff);

	FRsEffectCoefficient RsStaggerDamageCoeff(DeveloperSetting->StaggerDamageEffectClass, StaggerDamageCoefficients);
	ApplyInstantDamage(SourceASC, TargetASC, RsStaggerDamageCoeff);

	ApplyHitReaction(SourceASC, TargetASC);
}

void URsEffectDefinition_DotDamage::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	FRsEffectCoefficient RsHealthDotCoeff(DeveloperSetting->HealthDotDamageEffectClass, HealthDamageCoefficients);
	ApplyDotDamage(SourceASC, TargetASC, RsHealthDotCoeff, Duration, Period);
	
	FRsEffectCoefficient RsStaggerDotCoeff(DeveloperSetting->StaggerDotDamageEffectClass, StaggerDamageCoefficients);
	ApplyDotDamage(SourceASC, TargetASC, RsStaggerDotCoeff, Duration, Period);
	
	ApplyHitReaction(SourceASC, TargetASC);
}

void URsEffectDefinition_DotBurstDamage::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	// Apply DoT Burst Damage
	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	FGameplayEffectSpecHandle DotBurstDamageSpec = SourceASC->MakeOutgoingSpec(DeveloperSetting->DotBurstDamageEffectClass, 0.f, EffectContext);
	if (DotBurstDamageSpec.IsValid())
	{
		SET_SETBYCALLER_PROPERTY(DotBurstDamageSpec, InvinciblePierce);
		SET_SETBYCALLER_PROPERTY(DotBurstDamageSpec, DamageMultiplierPerDotStacks);
		DotBurstDamageSpec.Data->AppendDynamicAssetTags(DamageTags);
		SourceASC->ApplyGameplayEffectSpecToTarget(*DotBurstDamageSpec.Data, TargetASC);
	}
	
	ApplyHitReaction(SourceASC, TargetASC);
}

void URsEffectDefinition_Buff::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	// Apply buff effect
	FRsEffectCoefficient EffectCoefficient(BuffClass, Coefficients);
	FGameplayEffectSpecHandle BuffSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, EffectCoefficient, SourceASC->MakeEffectContext());
	if (BuffSpec.IsValid())
	{
		BuffSpec.Data->SetSetByCallerMagnitude(RsGameplayTags::MANUAL_DURATION, Duration);
		SourceASC->ApplyGameplayEffectSpecToTarget(*BuffSpec.Data, TargetASC);
	}
}

void URsEffectDefinition_ChangeCooldown::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
	TArray<FActiveGameplayEffectHandle> CooldownEffects = SourceASC->GetActiveEffects(Query);
	for (const FActiveGameplayEffectHandle& EffectHandle : CooldownEffects)
	{
		if (const FActiveGameplayEffect* CooldownEffect = SourceASC->GetActiveGameplayEffect(EffectHandle))
		{
			if (ModifingType == ECooldownModifingType::Add)
			{
				SourceASC->ModifyActiveEffectStartTime(EffectHandle, Amount);
			}
			else if (ModifingType == ECooldownModifingType::Set)
			{
				float TimeRemaining = CooldownEffect->GetTimeRemaining(GetWorld()->GetTimeSeconds());
				SourceASC->ModifyActiveEffectStartTime(EffectHandle, -TimeRemaining + Amount);
			}
		}
	}
}

void URsEffectDefinition_GainEnergy::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (SourceASC == nullptr)
	{
		return;
	}
	
	// Create a dynamic instant Gameplay Effect
	if (UGameplayEffect* GainEnergyGE = NewObject<UGameplayEffect>(SourceASC))
	{
		GainEnergyGE->DurationPolicy = EGameplayEffectDurationType::Instant;
		GainEnergyGE->Modifiers.SetNum(1);

		FGameplayModifierInfo& ModifierInfo = GainEnergyGE->Modifiers[0];
		ModifierInfo.ModifierMagnitude = FScalableFloat(Amount);
		ModifierInfo.ModifierOp = EGameplayModOp::Additive;
		ModifierInfo.Attribute = URsEnergySet::GetCurrentEnergyAttribute();

		// Apply a dynamic instant Gameplay Effect
		SourceASC->ApplyGameplayEffectToSelf(GainEnergyGE, 0.f, SourceASC->MakeEffectContext());
	}
}

void URsEffectDefinition_Lifesteal::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (SourceASC == nullptr || TargetASC == nullptr)
	{
		return;
	}
	
	// Create a dynamic instant Gameplay Effect
	if (UGameplayEffect* LifestealGE = NewObject<UGameplayEffect>(SourceASC))
	{
		LifestealGE->DurationPolicy = EGameplayEffectDurationType::Instant;
		LifestealGE->Modifiers.SetNum(1);

		FGameplayModifierInfo& ModifierInfo = LifestealGE->Modifiers[0];
		float Amount = TargetASC->GetNumericAttribute(URsHealthSet::GetHealthDamageAttribute());
		ModifierInfo.ModifierMagnitude = FScalableFloat(Amount);
		ModifierInfo.ModifierOp = EGameplayModOp::Additive;
		ModifierInfo.Attribute = URsHealthSet::GetCurrentHealthAttribute();

		// Apply a dynamic instant Gameplay Effect
		SourceASC->ApplyGameplayEffectToSelf(LifestealGE, 0.f, SourceASC->MakeEffectContext());
	}
}

void URsEffectDefinition_HitStop::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (SourceASC == nullptr || TargetASC == nullptr)
	{
		return;
	}
	
	if (SourceHitStopTime > 0.f && SourceASC->GetAnimatingAbility())
	{
		if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(SourceASC->GetAnimatingAbility(), SourceHitStopTime))
		{
			PauseMontageTask->ReadyForActivation();
		}
	}
	
	if (TargetHitStopTime > 0.f && TargetASC->GetAnimatingAbility())
	{
		if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(TargetASC->GetAnimatingAbility(), TargetHitStopTime, TargetHitStopTime))
		{
			PauseMontageTask->ReadyForActivation();
		}
	}
}

void URsEffectDefinition_Custom::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	// Apply custom effect
	FRsEffectCoefficient CustomCoeff(CustomEffect.EffectClass, CustomEffect.Coefficients);
	FGameplayEffectSpecHandle CustomSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, CustomCoeff, SourceASC->MakeEffectContext());
	if (CustomSpec.IsValid())
	{
		SourceASC->ApplyGameplayEffectSpecToTarget(*CustomSpec.Data, TargetASC);
	}
}
