// Copyright 2024 Team BH.


#include "RsCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Attributes/RsMovementSet.h"

void URsCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		MovementSet = ASC->GetSet<URsMovementSet>();
	}
}

float URsCharacterMovementComponent::GetMaxSpeed() const
{
	float MovementSpeedMultiplier = 1.f;
	if (MovementSet)
	{
		MovementSpeedMultiplier = MovementSet->GetMoveSpeedMultiplier();
	}
	return Super::GetMaxSpeed() * MovementSpeedMultiplier;
}
