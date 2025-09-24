// Copyright 2024 Team BH.


#include "RsHealthComponent.h"

#include "AbilitySystemGlobals.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/System/RsGameSettingDataAsset.h"

URsHealthComponent::URsHealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void URsHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URsHealthComponent, bIsDead);
}

void URsHealthComponent::Initialize(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetCurrentHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetBarrierAttribute()).AddUObject(this, &ThisClass::HandleBarrierChange);
	HealthSet = AbilitySystemComponent->GetSet<URsHealthSet>();
	if (HealthSet)
	{
		OnHealthChange.Broadcast(HealthSet->GetCurrentHealth(), HealthSet->GetCurrentHealth(), nullptr);
		OnBarrierChange.Broadcast(HealthSet->GetBarrier(), HealthSet->GetBarrier(), nullptr);
	}
	
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::HandleBarrierAdded);
}

void URsHealthComponent::ApplyDamageToBarriers(UAbilitySystemComponent* AbilitySystemComponent, float DamageAmount)
{
	// DamageAmount == value between Input Damage ~ Max Shield (Clamp already done in HealthSet)
	for (const FActiveGameplayEffectHandle& ShieldHandle : ActiveBarrierHandles)
	{
		float ShieldAmount = AbilitySystemComponent->GetGameplayEffectMagnitude(ShieldHandle, URsHealthSet::GetBarrierAttribute());
		float AccumulatedDamage = FMath::Abs(AbilitySystemComponent->GetNumericAttributeBase(URsHealthSet::GetBarrierAttribute()));
		float ShieldDamage = FMath::Min(DamageAmount,ShieldAmount - AccumulatedDamage);

		// Accumulate current handled Damage amount in BaseValue of Shield Attribute
		AbilitySystemComponent->ApplyModToAttribute(URsHealthSet::GetBarrierAttribute(), EGameplayModOp::Additive, -ShieldDamage);
		DamageAmount -= ShieldDamage;

		// if Sum of Damage(for this shield) reached To ShieldAmount, it's broken
		const bool bIsShieldBroken = FMath::IsNearlyEqual(AccumulatedDamage + ShieldDamage,ShieldAmount);
		if (bIsShieldBroken)
		{
			HandleBarrierBroke(ShieldHandle);
		}
		// break loop when Damage is handled completely through shield
		if (FMath::IsNearlyZero(DamageAmount))
		{
			break;
		}
	}
	for (const FActiveGameplayEffectHandle& RemovePendingShieldHandle : RemovePendingBarrierHandles)
	{
		ActiveBarrierHandles.Remove(RemovePendingShieldHandle);	
	}
	RemovePendingBarrierHandles.Empty();
}

float URsHealthComponent::GetCurrentHealth()
{
	if (HealthSet)
	{
		return HealthSet->GetCurrentHealth();
	}
	return 0.f;
}

float URsHealthComponent::GetMaxHealth()
{
	if (HealthSet)
	{
		return HealthSet->GetMaxHealth();
	}
	return 0.f;
}

float URsHealthComponent::GetBarrier()
{
	if (HealthSet)
	{
		return HealthSet->GetBarrier();
	}
	return 0.f;
}

void URsHealthComponent::HandleHealthChange(const FOnAttributeChangeData& ChangeData)
{
	AActor* Instigator = nullptr;
	if (const FGameplayEffectModCallbackData* EffectModData = ChangeData.GEModData)
	{
		const FGameplayEffectContextHandle& EffectContext = EffectModData->EffectSpec.GetEffectContext();
		if (EffectContext.IsValid())
		{
			Instigator = EffectContext.GetOriginalInstigator();
		}
	}
	OnHealthChange.Broadcast(ChangeData.OldValue, ChangeData.NewValue, Instigator);

	if (ChangeData.NewValue <= 0.f && bIsDead == false)
	{
		bIsDead = true;
		OnRep_bIsDead(false);
		GetOwner()->ForceNetUpdate();
	}
}

void URsHealthComponent::HandleBarrierChange(const FOnAttributeChangeData& ChangeData)
{
	AActor* Instigator = nullptr;
	if (const FGameplayEffectModCallbackData* EffectModData = ChangeData.GEModData)
	{
		const FGameplayEffectContextHandle& EffectContext = EffectModData->EffectSpec.GetEffectContext();
		if (EffectContext.IsValid())
		{
			Instigator = EffectContext.GetOriginalInstigator();
		}
	}
	OnBarrierChange.Broadcast(ChangeData.OldValue, ChangeData.NewValue, Instigator);
}

void URsHealthComponent::HandleBarrierAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer OutTags;
	GESpec.GetAllGrantedTags(OutTags);
	if (OutTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Effect.Buff.BP"))))
	{
		float ShieldMagnitude = AbilitySystemComponent->GetGameplayEffectMagnitude(ActiveEffectHandle, URsHealthSet::GetBarrierAttribute());
		AbilitySystemComponent->OnGameplayEffectRemoved_InfoDelegate(ActiveEffectHandle)->AddUObject(this, &ThisClass::HandleBarrierRemove, ShieldMagnitude);
		
		ActiveBarrierHandles.Add(ActiveEffectHandle);
		ActiveBarrierHandles.Sort([&AbilitySystemComponent](const FActiveGameplayEffectHandle& A, const FActiveGameplayEffectHandle& B)
		{
			const FActiveGameplayEffect* AShield = AbilitySystemComponent->GetActiveGameplayEffect(A);
			const FActiveGameplayEffect* BShield = AbilitySystemComponent->GetActiveGameplayEffect(B);
			if (AShield && BShield)
			{
				return AShield->GetEndTime() < BShield->GetEndTime();
			}
			return BShield != nullptr;;
		});
	}
}

void URsHealthComponent::HandleBarrierRemove(const FGameplayEffectRemovalInfo& RemovalInfo, float Magnitude)
{
	if (!RemovePendingBarrierHandles.Contains(RemovalInfo.ActiveEffect->Handle))
	{
		ActiveBarrierHandles.Remove(RemovalInfo.ActiveEffect->Handle);
	}
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		ASC->ApplyModToAttribute(URsHealthSet::GetBarrierAttribute(), EGameplayModOp::Additive, Magnitude);
	}
}

void URsHealthComponent::HandleBarrierBroke(const FActiveGameplayEffectHandle& BrokenBarrierHandle)
{
	// Order of shield must be observed.
	RemovePendingBarrierHandles.Add(BrokenBarrierHandle);
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		ASC->RemoveActiveGameplayEffect(BrokenBarrierHandle);
	}
}

void URsHealthComponent::OnRep_bIsDead(bool OldValue)
{
	if (OldValue == false && bIsDead == true)
	{
		if (UAbilitySystemComponent* ASC = HealthSet->GetOwningAbilitySystemComponent())
		{
			FGameplayEventData Payload;
			Payload.EventTag = URsGameSettingDataAsset::Get()->DeathAbilityTag;
			ASC->HandleGameplayEvent(Payload.EventTag, &Payload);
		}
		OnDeathStarted.Broadcast(GetOwner());
	}
}
