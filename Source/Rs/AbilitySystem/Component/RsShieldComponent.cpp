// Copyright 2024 Team BH.


#include "RsShieldComponent.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"

URsShieldComponent::URsShieldComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URsShieldComponent::Initialize(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}

	ASC = AbilitySystemComponent;
	ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::HandleShieldAdded);
	ShieldAttribute = URsHealthSet::GetShieldAttribute();
}

void URsShieldComponent::ApplyDamageToShields(float DamageAmount)
{
	// DamageAmount == value between Input Damage ~ Max Shield (Clamp already done in HealthSet)
	for (const FActiveGameplayEffectHandle& ShieldHandle : ActiveShieldHandles)
	{
		float ShieldAmount = ASC->GetGameplayEffectMagnitude(ShieldHandle,ShieldAttribute);
		float AccumulatedDamage = FMath::Abs(ASC->GetNumericAttributeBase(ShieldAttribute));
		float ShieldDamage = FMath::Min(DamageAmount,ShieldAmount - AccumulatedDamage);

		// Accumulate current handled Damage amount in BaseValue of Shield Attribute
		ASC->ApplyModToAttribute(ShieldAttribute, EGameplayModOp::Additive, -ShieldDamage);
		DamageAmount -= ShieldDamage;

		// if Sum of Damage(for this shield) reached To ShieldAmount, it's broken
		const bool bIsShieldBroken = FMath::IsNearlyEqual(AccumulatedDamage + ShieldDamage,ShieldAmount);
		if (bIsShieldBroken)
		{
			HandleShieldBroke(ShieldHandle);
		}
		// break loop when Damage is handled completely through shield
		if (FMath::IsNearlyZero(DamageAmount))
		{
			break;
		}
	}
	for (const FActiveGameplayEffectHandle& RemovePendingShieldHandle : RemovePendingShieldHandles)
	{
		ActiveShieldHandles.Remove(RemovePendingShieldHandle);	
	}
	RemovePendingShieldHandles.Empty();
}

void URsShieldComponent::HandleShieldAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer OutTags;
	GESpec.GetAllGrantedTags(OutTags);
	if (OutTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Effect.Buff.Shield"))))
	{
		AbilitySystemComponent->OnGameplayEffectRemoved_InfoDelegate(ActiveEffectHandle)->AddUObject(this, &ThisClass::HandleShieldRemove);

		ActiveShieldHandles.Add(ActiveEffectHandle);
		ActiveShieldHandles.Sort([&AbilitySystemComponent](const FActiveGameplayEffectHandle& A, const FActiveGameplayEffectHandle& B)
		{
			const FActiveGameplayEffect* AShield = AbilitySystemComponent->GetActiveGameplayEffect(A);
			const FActiveGameplayEffect* BShield = AbilitySystemComponent->GetActiveGameplayEffect(B);
			return AShield->GetEndTime() < BShield->GetEndTime();
		});
	}
}

void URsShieldComponent::HandleShieldRemove(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	if (!RemovePendingShieldHandles.Contains(RemovalInfo.ActiveEffect->Handle))
	{
		ActiveShieldHandles.Remove(RemovalInfo.ActiveEffect->Handle);
	}
	ASC->SetNumericAttributeBase(ShieldAttribute,0.f);
}

void URsShieldComponent::HandleShieldBroke(const FActiveGameplayEffectHandle& BrokenShieldHandle)
{
	// order of these tasks must be observed.
	RemovePendingShieldHandles.Add(BrokenShieldHandle);
	ASC->RemoveActiveGameplayEffect(BrokenShieldHandle);
}