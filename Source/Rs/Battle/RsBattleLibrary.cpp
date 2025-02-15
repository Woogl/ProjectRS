// Copyright 2024 Team BH.


#include "RsBattleLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
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

void URsBattleLibrary::ApplyDamageEffect(const AActor* SourceActor, const AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	
	if (SourceASC && TargetASC && GameplayEffectClass)
	{
		FGameplayEffectContextHandle DamageEffectContext = SourceASC->MakeEffectContext();
		if (DamageEffectContext.IsValid())
		{
			DamageEffectContext.AddOrigin(TargetActor->GetActorLocation());
		
			FGameplayEffectSpecHandle DamageEffectSpecHandle = SourceASC->MakeOutgoingSpec(GameplayEffectClass, 0, DamageEffectContext);
			SourceASC->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data.Get(), TargetASC);
		}
	}
}

void URsBattleLibrary::ApplyDamageEffectSpec(const AActor* SourceActor, const AActor* TargetActor, const FGameplayEffectSpecHandle& EffectHandle)
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

bool URsBattleLibrary::IsCriticalHitEffect(FGameplayEffectContextHandle& EffectContextHandle)
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
