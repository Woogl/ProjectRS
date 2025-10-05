// Copyright 2024 Team BH.


#include "RsEffectDefinition.h"

#include "AbilitySystemComponent.h"
#include "RsStatusEffect.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/System/RsDeveloperSetting.h"

void URsEffectDefinition_DamageBase::PostInitProperties()
{
	Super::PostInitProperties();

	// Cache a convenient pointer to the developer setting.
	DeveloperSetting = &URsDeveloperSetting::Get();
}

FActiveGameplayEffectHandle URsEffectDefinition_DamageBase::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FGameplayEventData& EventData)
{
	return Super::ApplyEffect(SourceASC, TargetASC, EventData);
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

	EffectContext.AddInstigator(SourceActor, nullptr);
	EffectContext.AddHitResult(HitResult);
	return EffectContext;
}

void URsEffectDefinition_DamageBase::ApplyInstantDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FRsEffectCoefficient& RsCoeff)
{
	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	FGameplayEffectSpecHandle DamageSpec = URsAbilitySystemLibrary::MakeEffectSpecCoefficient(SourceASC, RsCoeff, EffectContext);
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
	FGameplayEffectSpecHandle DotDamageSpec = URsAbilitySystemLibrary::MakeEffectSpecCoefficient(SourceASC, RsCoeff, EffectContext);
	if (DotDamageSpec.IsValid())
	{
		DotDamageSpec.Data->SetSetByCallerMagnitude(RsGameplayTags::COEFFICIENT_MANUAL, Duration);
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
	HealthDamageCoefficients.Add(RsGameplayTags::COEFFICIENT_ATK_SOURCE, 1.f);
	StaggerDamageCoefficients.Add(RsGameplayTags::COEFFICIENT_IMP_SOURCE, 1.f);
}

FActiveGameplayEffectHandle URsEffectDefinition_InstantDamage::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FGameplayEventData& EventData)
{
	FRsEffectCoefficient RsHealthDamageCoeff(DeveloperSetting->HealthDamageEffectClass, HealthDamageCoefficients);
	ApplyInstantDamage(SourceASC, TargetASC, RsHealthDamageCoeff);

	FRsEffectCoefficient RsStaggerDamageCoeff(DeveloperSetting->StaggerDamageEffectClass, StaggerDamageCoefficients);
	ApplyInstantDamage(SourceASC, TargetASC, RsStaggerDamageCoeff);

	ApplyHitReaction(SourceASC, TargetASC);

	return FActiveGameplayEffectHandle();
}

FActiveGameplayEffectHandle URsEffectDefinition_DotDamage::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FGameplayEventData& EventData)
{
	FRsEffectCoefficient RsHealthDotCoeff(DeveloperSetting->HealthDotDamageEffectClass, HealthDamageCoefficients);
	ApplyDotDamage(SourceASC, TargetASC, RsHealthDotCoeff, Duration, Period);
	
	FRsEffectCoefficient RsStaggerDotCoeff(DeveloperSetting->StaggerDotDamageEffectClass, StaggerDamageCoefficients);
	ApplyDotDamage(SourceASC, TargetASC, RsStaggerDotCoeff, Duration, Period);
	
	ApplyHitReaction(SourceASC, TargetASC);

	return FActiveGameplayEffectHandle();
}

FActiveGameplayEffectHandle URsEffectDefinition_DotBurstDamage::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FGameplayEventData& EventData)
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

	return FActiveGameplayEffectHandle();
}

FActiveGameplayEffectHandle URsEffectDefinition_RsGameplayEffect::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FGameplayEventData& EventData)
{
	FGameplayEffectSpecHandle EffectSpec = SourceASC->MakeOutgoingSpec(Effect, 0.f, SourceASC->MakeEffectContext());
	if (EventData.EventMagnitude > 0)
	{
		EffectSpec.Data->SetDuration(EventData.EventMagnitude, true);
	}
	return SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data, TargetASC);
}

FActiveGameplayEffectHandle URsEffectDefinition_Custom::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FGameplayEventData& EventData)
{
	FGameplayEffectSpecHandle EffectSpec = SourceASC->MakeOutgoingSpec(Effect, 0.f, SourceASC->MakeEffectContext());
	if (EventData.EventMagnitude > 0)
	{
		EffectSpec.Data->SetDuration(EventData.EventMagnitude, true);
	}
	if (!SetByCallerTags.IsEmpty())
	{
		for (const TTuple<FGameplayTag, float>& Data : SetByCallerTags)
		{
			EffectSpec.Data->SetSetByCallerMagnitude(Data.Key, Data.Value);
		}
	}
	if (!SetByCallerNames.IsEmpty())
	{
		for (const TTuple<FName, float>& Data : SetByCallerNames)
		{
			EffectSpec.Data->SetSetByCallerMagnitude(Data.Key, Data.Value);
		}
	}
	return SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data, TargetASC);
}
