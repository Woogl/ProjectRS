// Copyright 2024 Team BH.


#include "RsBattleLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"
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

void URsBattleLibrary::ApplyDamageEffect(const AActor* SourceActor, const AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, FGameplayTagContainer AdditionalDamageEffectTags)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);
	
	if (SourceASC && GameplayEffectClass)
	{
		FGameplayEffectContextHandle DamageEffectContext = SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle DamageEffectSpecHandle = SourceASC->MakeOutgoingSpec(GameplayEffectClass, 0, DamageEffectContext);
		ApplyDamageEffectSpec(SourceActor, TargetActor, DamageEffectSpecHandle, AdditionalDamageEffectTags);
	}
}

void URsBattleLibrary::ApplyDamageEffectSpec(const AActor* SourceActor, const AActor* TargetActor, const FGameplayEffectSpecHandle& EffectHandle, FGameplayTagContainer AdditionalDamageEffectTags)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	
	if (SourceASC && TargetASC)
	{
		if (FGameplayEffectSpec* EffectSpec = EffectHandle.Data.Get())
		{
			EffectSpec->GetContext().AddOrigin(TargetActor->GetActorLocation());
			EffectSpec->AppendDynamicAssetTags(AdditionalDamageEffectTags);
			SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpec, TargetASC);

			if (URsAbilitySystemComponent* RsASC = Cast<URsAbilitySystemComponent>(SourceASC))
			{
				RsASC->OnDealDamage.Broadcast(TargetASC, EffectHandle);
			}
		}
	}
}

void URsBattleLibrary::ApplyEffectCoefficient(const AActor* SourceActor, const AActor* TargetActor, FRsEffectCoefficient EffectCoefficient)
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
		ApplyEffectSpecCoefficient(SourceActor, TargetActor, EffectSpecHandle);
	}
}

void URsBattleLibrary::ApplyEffectSpecCoefficient(const AActor* SourceActor, const AActor* TargetActor, const FGameplayEffectSpecHandle& EffectHandle)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	
	if (SourceASC && TargetASC)
	{
		if (FGameplayEffectSpec* EffectSpec = EffectHandle.Data.Get())
		{
			EffectSpec->GetContext().AddOrigin(TargetActor->GetActorLocation());
			SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpec, TargetASC);
		}
	}
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
