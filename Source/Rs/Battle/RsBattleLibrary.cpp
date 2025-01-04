// Copyright 2024 Team BH.


#include "RsBattleLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "TargetingSystem/TargetingSubsystem.h"

TArray<AActor*> URsBattleLibrary::ExecuteTargeting(AActor* SourceActor, const UTargetingPreset* TargetingPreset, bool& bFound)
{
	if (SourceActor == nullptr || TargetingPreset == nullptr)
	{
		return TArray<AActor*>();
	}
	
	UTargetingSubsystem* TargetingSubsystem = UTargetingSubsystem::Get(SourceActor->GetWorld());
	if (TargetingSubsystem == nullptr)
	{
		return TArray<AActor*>();
	}
	
	FTargetingSourceContext Context;
	Context.SourceActor = SourceActor;
	Context.SourceObject = SourceActor;
	Context.SourceLocation = SourceActor->GetActorLocation();
	
	FTargetingRequestHandle Handle = TargetingSubsystem->MakeTargetRequestHandle(TargetingPreset, Context);
	TargetingSubsystem->ExecuteTargetingRequestWithHandle(Handle);
	
	TArray<FHitResult> HitResults;
	TargetingSubsystem->GetTargetingResults(Handle, HitResults);
	
	TArray<AActor*> TargetingResultActors;
	for (const FHitResult& HitResult : HitResults)
	{
		TargetingResultActors.Add(HitResult.GetActor());
	}

	TargetingSubsystem->ReleaseTargetRequestHandle(Handle);
	bFound = !TargetingResultActors.IsEmpty();
	
	return TargetingResultActors;
}

void URsBattleLibrary::ApplyDamageEffect(AActor* SourceActor, AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	
	if (SourceASC && TargetASC && GameplayEffectClass)
	{
		UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
		SourceASC->ApplyGameplayEffectToTarget(GameplayEffect, TargetASC);
	}
}
