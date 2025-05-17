// Copyright 2024 Team BH.


#include "RsBattleLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility_Attack.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Effect/RsDamageDefinition.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"
#include "TargetingSystem/TargetingSubsystem.h"

bool URsBattleLibrary::ExecuteTargeting(AActor* SourceActor, const UTargetingPreset* TargetingPreset, TArray<AActor*>& ResultActors)
{
	if (SourceActor == nullptr || TargetingPreset == nullptr)
	{
		return false;
	}
	
	UTargetingSubsystem* TargetingSubsystem = UTargetingSubsystem::Get(SourceActor->GetWorld());
	if (TargetingSubsystem == nullptr)
	{
		return false;
	}
	
	FTargetingSourceContext Context;
	Context.SourceActor = SourceActor;
	Context.InstigatorActor = SourceActor;
	Context.SourceObject = SourceActor;
	Context.SourceLocation = SourceActor->GetActorLocation();
	
	FTargetingRequestHandle Handle = TargetingSubsystem->MakeTargetRequestHandle(TargetingPreset, Context);
	TargetingSubsystem->ExecuteTargetingRequestWithHandle(Handle);
	
	TArray<FHitResult> HitResults;
	TargetingSubsystem->GetTargetingResults(Handle, HitResults);
	
	for (const FHitResult& HitResult : HitResults)
	{
		ResultActors.Add(HitResult.GetActor());
	}

	TargetingSubsystem->ReleaseTargetRequestHandle(Handle);
	
	return !ResultActors.IsEmpty();
}

FGameplayEffectSpecHandle URsBattleLibrary::MakeEffectSpecCoefficient(UAbilitySystemComponent* SourceASC, const FRsEffectCoefficient& EffectCoefficient, FGameplayEffectContextHandle InEffectContext)
{
	if (SourceASC && EffectCoefficient.IsValid())
	{
		FGameplayEffectContextHandle EffectContext = InEffectContext.IsValid() ? InEffectContext : SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(EffectCoefficient.EffectClass, 0, EffectContext);
		for (const TTuple<FGameplayTag, float>& Coefficient : EffectCoefficient.Coefficients)
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(Coefficient.Key, Coefficient.Value);
		}
		return EffectSpecHandle;
	}
	return FGameplayEffectSpecHandle();
}

void URsBattleLibrary::ApplyDamageContext(const AActor* Source, const AActor* Target, FRsDamageContext& DamageContext)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Source);
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (!SourceASC || !TargetASC)
	{
		return;
	}

	// Can be immune by RsInvincibleGEComp or RsSuperArmorGECpomp.
	for (TObjectPtr<URsDamageDefinition> DamageDefinition : DamageContext.DamageDefinitions)
	{
		DamageDefinition->ApplyDamage(SourceASC, TargetASC);
	}
}

FActiveGameplayEffectHandle URsBattleLibrary::ApplyEffectCoefficient(const AActor* Source, const AActor* Target, const FRsEffectCoefficient& EffectCoefficient)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Source);
	
	if (SourceASC && EffectCoefficient.IsValid())
	{
		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(EffectCoefficient.EffectClass, 0, EffectContext);
		for (const TTuple<FGameplayTag, float>& Coefficient : EffectCoefficient.Coefficients)
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(Coefficient.Key, Coefficient.Value);
		}
		return ApplyEffectSpecCoefficient(Source, Target, EffectSpecHandle);
	}
	return FActiveGameplayEffectHandle();
}

FActiveGameplayEffectHandle URsBattleLibrary::ApplyEffectSpecCoefficient(const AActor* Source, const AActor* Target, const FGameplayEffectSpecHandle& EffectHandle)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Source);
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	
	if (SourceASC && TargetASC)
	{
		if (FGameplayEffectSpec* EffectSpec = EffectHandle.Data.Get())
		{
			EffectSpec->GetContext().AddOrigin(Target->GetActorLocation());
			return SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpec, TargetASC);
		}
	}
	return FActiveGameplayEffectHandle();
}

bool URsBattleLibrary::IsCriticalHitEffect(FGameplayEffectContextHandle EffectContextHandle)
{
	if (FGameplayEffectContext* EffectContext = EffectContextHandle.Get())
	{
		if (EffectContext->GetScriptStruct() == FRsGameplayEffectContext::StaticStruct())
		{
			FRsGameplayEffectContext RsEffectContext = *static_cast<FRsGameplayEffectContext*>(EffectContextHandle.Get());
			return RsEffectContext.bIsCriticalHit;
		}
	}
	return false;
}

bool URsBattleLibrary::IsDead(const AActor* Target)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target))
	{
		return ASC->GetNumericAttribute(URsHealthSet::GetCurrentHealthAttribute()) <= 0.f;
	}
	return false;
}
