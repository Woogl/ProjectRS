// Copyright 2024 Team BH.


#include "RsCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/Attributes/RsSpeedSet.h"
#include "Rs/System/RsGameSettingDataAsset.h"

void URsCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		SpeedSet = ASC->GetSet<URsSpeedSet>();
	}
}

float URsCharacterMovementComponent::GetMaxSpeed() const
{
	float MovementSpeedMultiplier = 1.f;
	if (SpeedSet)
	{
		MovementSpeedMultiplier = SpeedSet->GetMoveSpeed();
	}
	return Super::GetMaxSpeed() * MovementSpeedMultiplier;
}

void URsCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if (IsFalling())
		{
			ASC->AddLooseGameplayTag(RsGameplayTags::MOVEMENT_FALLING);
		}
		else
		{
			ASC->RemoveLooseGameplayTag(RsGameplayTags::MOVEMENT_FALLING);
		}
	}
}
