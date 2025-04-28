// Copyright 2024 Team BH.


#include "RsBattleLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility_Attack.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Effect/RsDamageEffect.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"
#include "Rs/System/RsDeveloperSetting.h"
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

FGameplayEffectSpecHandle URsBattleLibrary::MakeEffectSpecCoefficient(const AActor* SourceActor, const AActor* TargetActor, FRsEffectCoefficient EffectCoefficient)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);
	
	if (SourceASC && EffectCoefficient.EffectClass)
	{
		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(EffectCoefficient.EffectClass, 0, EffectContext);
		for (const TTuple<FGameplayTag, float>& Coefficient : EffectCoefficient.Coefficients)
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(Coefficient.Key, Coefficient.Value);
		}
		return EffectSpecHandle;
	}
	return FGameplayEffectSpecHandle();
}

FActiveGameplayEffectHandle URsBattleLibrary::ApplyDamageContext(const AActor* SourceActor, const AActor* TargetActor, FRsDamageContext& DamageContext)
{
	InternalSortDamageEffectsByOrder(DamageContext);
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	if (SourceASC && TargetASC)
	{
		FGameplayEffectContextHandle DamageEffectContext = SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle DamageEffectHandle = SourceASC->MakeOutgoingSpec(URsDamageEffect::StaticClass(), 0, DamageEffectContext);
		DamageEffectHandle.Data->SetSetByCallerMagnitude(URsDeveloperSetting::Get()->ManualLevelTag, DamageContext.InvinciblePenetrationLevel);

		TArray<FGameplayEffectSpecHandle> AdditionalEffects;
		for (FRsEffectCoefficient EffectCoefficient : DamageContext.EffectCoefficients)
		{
			AdditionalEffects.Add(MakeEffectSpecCoefficient(SourceActor,TargetActor,EffectCoefficient));
		}
		DamageEffectHandle.Data->TargetEffectSpecs = AdditionalEffects;
		return SourceASC->ApplyGameplayEffectSpecToTarget(*DamageEffectHandle.Data,TargetASC);
	}
	return FActiveGameplayEffectHandle();
}

void URsBattleLibrary::InternalSortDamageEffectsByOrder(FRsDamageContext& DamageContexts)
{
	for (const FRsEffectCoefficient& Context : DamageContexts.EffectCoefficients)
	{
		if (URsDeveloperSetting::Get()->DamageEffectApplicationOrder.Find(Context.EffectClass) != INDEX_NONE)
		{
			DamageContexts.EffectCoefficients.Sort([Order = URsDeveloperSetting::Get()->DamageEffectApplicationOrder](const FRsEffectCoefficient& A, const FRsEffectCoefficient& B)
			{
				TArray<FRsEffectCoefficient>::SizeType OrderA = Order.Find(A.EffectClass);
				TArray<FRsEffectCoefficient>::SizeType OrderB = Order.Find(B.EffectClass);

				if (OrderA == INDEX_NONE){ return false; }
				if (OrderB == INDEX_NONE){ return true; }
				return OrderA < OrderB;
			});
			return;
		}
	}
}

FActiveGameplayEffectHandle URsBattleLibrary::ApplyEffectCoefficient(const AActor* SourceActor, const AActor* TargetActor, FRsEffectCoefficient EffectCoefficient)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);
	
	if (SourceASC && EffectCoefficient.EffectClass)
	{
		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(EffectCoefficient.EffectClass, 0, EffectContext);
		for (const TTuple<FGameplayTag, float>& Coefficient : EffectCoefficient.Coefficients)
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(Coefficient.Key, Coefficient.Value);
		}
		return ApplyEffectSpecCoefficient(SourceActor, TargetActor, EffectSpecHandle);
	}
	return FActiveGameplayEffectHandle();
}

FActiveGameplayEffectHandle URsBattleLibrary::ApplyEffectSpecCoefficient(const AActor* SourceActor, const AActor* TargetActor, const FGameplayEffectSpecHandle& EffectHandle)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	
	if (SourceASC && TargetASC)
	{
		if (FGameplayEffectSpec* EffectSpec = EffectHandle.Data.Get())
		{
			EffectSpec->GetContext().AddOrigin(TargetActor->GetActorLocation());
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
