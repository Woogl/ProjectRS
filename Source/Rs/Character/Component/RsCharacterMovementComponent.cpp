// Copyright 2024 Team BH.


#include "RsCharacterMovementComponent.h"

#include "AbilitySystemGlobals.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/Attributes/RsSpeedSet.h"

void URsCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UAbilitySystemComponent* ASC = TryGetAbilitySystemComponent())
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

	if (IsFalling())
	{
		AddMovementTag(RsGameplayTags::MOVEMENT_FALLING);
		RemoveMovementTag(RsGameplayTags::MOVEMENT_GROUNDED);
	}
	else
	{
		AddMovementTag(RsGameplayTags::MOVEMENT_GROUNDED);
		RemoveMovementTag(RsGameplayTags::MOVEMENT_FALLING);
	}
}

void URsCharacterMovementComponent::AddMovementTag(const FGameplayTag MovementTag)
{
	if (UAbilitySystemComponent* ASC = TryGetAbilitySystemComponent())
	{
		if (!ASC->HasMatchingGameplayTag(MovementTag))
		{
			ASC->AddLooseGameplayTag(MovementTag);
		}
	}
}

void URsCharacterMovementComponent::RemoveMovementTag(const FGameplayTag MovementTag)
{
	if (UAbilitySystemComponent* ASC = TryGetAbilitySystemComponent())
	{
		if (ASC->HasMatchingGameplayTag(MovementTag))
		{
			ASC->RemoveLooseGameplayTag(MovementTag);
		}
	}
}

UAbilitySystemComponent* URsCharacterMovementComponent::TryGetAbilitySystemComponent() const
{
	if (CachedASC)
	{
		return CachedASC;
	}
	else
	{
		CachedASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
		return CachedASC;
	}
}
