// Copyright 2024 Team BH.


#include "RsBattleLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "RsBattleSubsystem.h"

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
		LinkSkillType = BattleSubsystem->GetLinkSkillType();
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
