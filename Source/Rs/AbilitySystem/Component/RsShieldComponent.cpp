// Copyright 2024 Team BH.


#include "RsShieldComponent.h"

#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"

URsShieldComponent::URsShieldComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URsShieldComponent::Initialize(UAbilitySystemComponent* ASC)
{
	if (ASC == nullptr)
	{
		return;
	}

	AbilitySystemComponent = ASC;
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::HandleShieldAdded);
}

void URsShieldComponent::ApplyDamageToShields(float DamageAmount)
{
	float LocalDamageAmount = DamageAmount;
	for (const FActiveGameplayEffectHandle& Shield : ActiveShieldHandles)
	{
		float ShieldAmount = ActiveShieldAmounts.FindRef(Shield);
		float ShieldDamage = FMath::Clamp(LocalDamageAmount, 0.0f, ShieldAmount);
		ShieldAmount = FMath::Max(0.f, ShieldAmount - ShieldDamage);
		
		AbilitySystemComponent->ApplyModToAttribute(URsHealthSet::GetShieldAttribute(), EGameplayModOp::Additive, -ShieldDamage);
		ActiveShieldAmounts[Shield] = ShieldAmount;
		
		LocalDamageAmount -= ShieldDamage;
		if (LocalDamageAmount <= 0)
		{
			break;
		}
	}

	TArray<FActiveGameplayEffectHandle> PendingRemoveShields;
	for	(TTuple<FActiveGameplayEffectHandle, float>& ActiveShield : ActiveShieldAmounts)
	{
		if (ActiveShield.Value <= 0.f)
		{
			PendingRemoveShields.Add(ActiveShield.Key);
		}
	}
	for (FActiveGameplayEffectHandle PendingRemoveShield : PendingRemoveShields)
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(PendingRemoveShield, 1);
	}
}

void URsShieldComponent::HandleShieldAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer OutTags;
	GESpec.GetAllGrantedTags(OutTags);
	if (OutTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Effect.Buff.Shield"))))
	{
		ASC->OnGameplayEffectRemoved_InfoDelegate(ActiveHandle)->AddUObject(this, &ThisClass::HandleShieldRemoved);

		float ShieldAmount = AbilitySystemComponent->GetGameplayEffectMagnitude(ActiveHandle, URsHealthSet::GetShieldAttribute());
		ActiveShieldAmounts.Add(ActiveHandle, ShieldAmount);
		ActiveShieldHandles.Add(ActiveHandle);
		ActiveShieldHandles.Sort([&ASC](const FActiveGameplayEffectHandle& A, const FActiveGameplayEffectHandle& B)
		{
			const FActiveGameplayEffect* AShield = ASC->GetActiveGameplayEffect(A);
			const FActiveGameplayEffect* BShield = ASC->GetActiveGameplayEffect(B);
			return AShield->GetEndTime() < BShield->GetEndTime();
		});
	}
}

void URsShieldComponent::HandleShieldRemoved(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	ActiveShieldHandles.Remove(RemovalInfo.ActiveEffect->Handle);
	ActiveShieldAmounts.Remove(RemovalInfo.ActiveEffect->Handle);
	if (ActiveShieldHandles.IsEmpty())
	{
		AbilitySystemComponent->SetNumericAttributeBase(URsHealthSet::GetShieldAttribute(), 0.f);
	}
}

