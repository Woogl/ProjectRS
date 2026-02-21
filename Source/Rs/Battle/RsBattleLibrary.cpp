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
