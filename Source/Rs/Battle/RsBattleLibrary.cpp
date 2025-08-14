// Copyright 2024 Team BH.


#include "RsBattleLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Effect/RsEffectDefinition.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "Subsystem/RsBattleSubsystem.h"

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

bool URsBattleLibrary::IsDotDamageEffect(FGameplayEffectContextHandle EffectContextHandle)
{
	if (FGameplayEffectContext* EffectContext = EffectContextHandle.Get())
	{
		if (EffectContext->GetScriptStruct() == FRsGameplayEffectContext::StaticStruct())
		{
			FRsGameplayEffectContext RsEffectContext = *static_cast<FRsGameplayEffectContext*>(EffectContextHandle.Get());
			return RsEffectContext.bIsDotDamage;
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

AActor* URsBattleLibrary::GetLockOnTarget(APawn* Pawn)
{
	if (!Pawn)
	{
		return nullptr;
	}
	AController* Controller = Pawn->GetController();
	if (!Controller)
	{
		return nullptr;
	}
	
	if (URsLockOnComponent* LockOnComponent = Controller->FindComponentByClass<URsLockOnComponent>())
	{
		return LockOnComponent->GetLockOnTarget();
	}

	return nullptr;
}

void URsBattleLibrary::GetLinkSkillInfo(UObject* WorldContextObject, ARsCharacterBase*& LinkSkillTarget, ERsLinkSkillType& LinkSkillType, int32& AvailableCount)
{
	if (URsBattleSubsystem* BattleSubsystem = URsBattleSubsystem::Get(WorldContextObject))
	{
		LinkSkillTarget = BattleSubsystem->GetLinkSkillTarget();
		LinkSkillType = BattleSubsystem->GetLastLinkSkillType();
		AvailableCount = BattleSubsystem->GetAvailableLinkSkillCount();
	}
}

void URsBattleLibrary::SetLinkSkillTarget(UObject* WorldContextObject, ARsCharacterBase* LinkSkillTarget, ERsLinkSkillType Type)
{
	if (URsBattleSubsystem* BattleSubsystem = URsBattleSubsystem::Get(WorldContextObject))
	{
		BattleSubsystem->SetLinkSkillTarget(LinkSkillTarget, Type);
	}
}

void URsBattleLibrary::DecrementLinkSkillTarget(UObject* WorldContextObject, ARsCharacterBase* LinkSkillTarget, ERsLinkSkillType Type)
{
	if (URsBattleSubsystem* BattleSubsystem = URsBattleSubsystem::Get(WorldContextObject))
	{
		BattleSubsystem->DecrementLinkSkillCount(LinkSkillTarget, Type);
	}
}
