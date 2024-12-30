// Copyright 2024 Team BH.


#include "RsCharacterBase.h"

#include "Net/UnrealNetwork.h"
#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"

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
