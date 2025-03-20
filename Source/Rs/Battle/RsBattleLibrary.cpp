// Copyright 2024 Team BH.


#include "RsBattleLibrary.h"

#include <ThirdParty/OpenSubdiv/Deploy/OpenSubdiv-3.6.0/include/opensubdiv/vtr/types.h>

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Net/RepLayout.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"
#include "Rs/Character/RsCharacterBase.h"
#include "Rs/System/RsGameSetting.h"
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

TArray<FGameplayEffectSpecHandle> URsBattleLibrary::MakeDamageEffectSpecs(const AActor* SourceActor, const TArray<FRsDamageEffectParams>& Params)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);

	TArray<FGameplayEffectSpecHandle> OutResult;
	if (SourceASC)
	{
		TArray<FRsDamageEffectData> DamageEffectList = GetEffectDataList(Params);
		for (const FRsDamageEffectData& Data : DamageEffectList)
		{
			FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
			OutResult.Add(SourceASC->MakeOutgoingSpec(Data.EffectClass,Data.EffectLevel,ContextHandle));
		}
	}
	return OutResult;
}

bool URsBattleLibrary::ApplyDamageEffectSpecs(const AActor* SourceActor, const AActor* TargetActor,	const TArray<FGameplayEffectSpecHandle>& Handles)
{
	if (Handles.IsEmpty())
	{
		return false;
	}

	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);

	if (SourceASC && TargetASC)
	{
		for (const FGameplayEffectSpecHandle& Handle : Handles)
		{
			Handle.Data->GetContext().AddOrigin(TargetActor->GetActorLocation());
			SourceASC->ApplyGameplayEffectSpecToTarget(*Handle.Data,TargetASC);
		}
		if (URsAbilitySystemComponent* RsASC = Cast<URsAbilitySystemComponent>(SourceASC))
		{
			RsASC->OnDealDamage.Broadcast(TargetASC, Handles[0]);
		}
		return true;
	}
	return false;
}

TArray<FRsDamageEffectData> URsBattleLibrary::GetEffectDataList(const TArray<FRsDamageEffectParams>& Params)
{
	TArray<FRsDamageEffectData> OutResult;

	for (const FRsDamageEffectParams& Param : Params)
	{
		if (URsGameSetting::Get()->DamageEffectTagTable.Find(Param.EffectTag) != INDEX_NONE)
		{
			int32 Index = URsGameSetting::Get()->DamageEffectTagTable.Find(Param.EffectTag);
			FRsEffectTagData* EffectTagData = &URsGameSetting::Get()->DamageEffectTagTable[Index];
			
			FRsDamageEffectData DamageEffectData(Index, EffectTagData->EffectClass,Param.EffectLevel);
			OutResult.Add(DamageEffectData);
		}
	}
	OutResult.Sort([](const FRsDamageEffectData& Data1, const FRsDamageEffectData& Data2)->bool { return Data1.Order < Data2.Order; });
	return OutResult;
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

bool URsBattleLibrary::IsDead(const ARsCharacterBase* Character)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character))
	{
		return ASC->GetNumericAttribute(URsHealthSet::GetCurrentHealthAttribute()) <= 0.f;
	}
	return false;
}
