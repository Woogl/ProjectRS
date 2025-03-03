// Copyright 2024 Team BH.


#include "RsCharacterBase.h"

#include "Net/UnrealNetwork.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"
#include "Rs/Battle/RsBattleLibrary.h"

void ARsCharacterBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TeamID);
}

UAbilitySystemComponent* ARsCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

URsAbilitySystemComponent* ARsCharacterBase::GetRsAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool ARsCharacterBase::IsLockableTarget_Implementation() const
{
	if (URsBattleLibrary::IsDead(this))
	{
		return false;
	}
	return true;
}
